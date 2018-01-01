#include <stdio.h>
#include <stdlib.h>
#include "cJSON.h"
#include "errlog.h"
#include "vm.h"

int  load_config(struct conf *config);
int  get_config(const char *config_json, struct conf *config);
int  create_default_config(struct conf *config);
char *read_file_content(FILE *fp);
void init_heap(size_t size);
int  init_stack(size_t size);
int  load_bytecode_file(const char *src);
void free_stack();
void free_heap();

int load(const char *src) {

    struct conf config;
    int err;

    LOG_ERROR(NO_ERROR, OK); 

    err = load_config(&config);    
    if (err == 0) {
        init_heap(config.heap_capacity);
        err = init_stack(config.stack_capacity);
        if (err == 0) {
            return load_bytecode_file(src);
        } else {
            return -1;
        }
    } else {
        return -1;
    }
}

int load_config(struct conf *config) {

    FILE *fp = NULL; 
    struct conf ret;
    char *config_string = NULL;
    int err;

    fp = fopen("./vm.conf", "r");
    if (fp == NULL) {
        create_default_config(&ret);
    } else {
        config_string = read_file_content(fp);
        if (config_string == NULL) {
            goto error;
        }

        if (get_config(config_string, &ret) != 0) {
            goto error;
        }

        fclose(fp);
        *config = ret;
        free(config_string);
    }
    
    return 0;

/* clean up */
error:
    if (fp != NULL) {
        fclose(fp);
    }
    if (config_string != NULL) {
        free(config_string);
    }
    return -1;
}

int get_config(const char *config_json, struct conf *config) {

    unsigned long heap_capacity, stack_capacity;
    cJSON *json_conf, *json_heap_capacity, *json_stack_capacity;
    json_conf = json_heap_capacity = json_stack_capacity = NULL;
    
    json_conf = cJSON_Parse(config_json);
    if (json_conf == NULL) {
        LOG_ERROR(CONFIG_ERROR, ILLEGAL_CONFIG_FILE_FORMAT);
        goto error;
    }

    json_heap_capacity  = cJSON_GetObjectItemCaseSensitive(json_conf, "heap_capacity");
    json_stack_capacity = cJSON_GetObjectItemCaseSensitive(json_conf, "stack_capacity");
     
    /* verify configuration */
    if (!cJSON_IsNumber(json_heap_capacity)) {
        LOG_ERROR(CONFIG_ERROR, HEAP_CAPACITY_NOT_A_NUMBER);
        goto error;
    }
    else if (!cJSON_IsNumber(json_stack_capacity)) {
        LOG_ERROR(CONFIG_ERROR, STACK_CAPACITY_NOT_A_NUMBER);
        goto error;
    }
   
    heap_capacity = (unsigned long)json_heap_capacity->valuedouble;
    stack_capacity = (unsigned long)json_stack_capacity->valuedouble;

    if (heap_capacity <= MIN_HEAP_CAPACITY || heap_capacity >= MAX_HEAP_CAPACITY) {
        LOG_ERROR(CONFIG_ERROR, HEAP_CAPACITY_OUT_OF_RANGE);
        goto error;
    }
    else if (stack_capacity <= MIN_STACK_CAPACITY || stack_capacity >= MAX_STACK_CAPACITY) {
        LOG_ERROR(CONFIG_ERROR, STACK_CAPACITY_OUT_OF_RANGE);
        goto error;
    } 

    config->heap_capacity = heap_capacity;
    config->stack_capacity = stack_capacity;

    return 0;

error:
    if (json_conf != NULL) {
       cJSON_Delete(json_conf);
    }

    return -1;
}

char *read_file_content(FILE *fp) {

    long length;
    char *buffer; 
    
    fseek(fp, 0, SEEK_END);
    length = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    
    buffer = (char *)malloc(length * sizeof(char));

    if (buffer != NULL) {
        fread(buffer, 1, length, fp);
    }

    return buffer;
}

int create_default_config(struct conf *config) {

    char *rendered;
    cJSON *json_conf;
    FILE *fp = fopen("./vm.conf","w");

    if (fp != NULL) {
        json_conf = cJSON_CreateObject();
        cJSON_AddNumberToObject(json_conf, "heap_capacity", default_config.heap_capacity);
        cJSON_AddNumberToObject(json_conf, "stack_capacity", default_config.stack_capacity);
        rendered = cJSON_Print(json_conf); 
        fputs(rendered, fp);

        fclose(fp);
        free(rendered);
        return 0;
    } else {
        LOG_ERROR(CONFIG_ERROR, CANNOT_OPEN_CONFIG_FILE);

        return -1;
    }
}
 
void init_heap(size_t size) {
    vm.heap.survive_flag = DEFAULT_SURVIVE_FLAG;
    vm.heap.current_size = 0;
    vm.heap.max_size = size;
    vm.heap.list = NULL;
}

int init_stack(size_t size) {
    vm.stack.capacity = size; 
    vm.stack.length = 0;
    vm.stack.stack = malloc(size);
    if (vm.stack.stack == NULL) {
        return -1;
    }

    vm.registers.sp = vm.stack.stack;
    vm.registers.bp = vm.registers.hr = NULL;

    return 0;
}

/*
 * RAWCODE FILE'S STRUCTURE 
 * [OFFSETS] [CONSTANT POOL] [INSTRUCTION LIST]
 *
 * OFFSETS
 * [ constant pool start offset <unsigned long int> ] [ instruction start offset <unsigned long int> ] ...
 * CONSTANT POOL'S STRUCTURE IN RAWCODE
 * ... [ count number <unsigne long int> ] [ array of constant offset <unsigned long int> ] [ constants ] ...
 *
 * INSTRUCTION LIST'S STRUCTURE IN RAWCODE 
 * ... [ list length <unsigned long int> ] [ array of instr <char> ] ... 
 *
 */
int load_bytecode_file(const char *src) {
   
    /* read bytecode file */  
    FILE *fp = NULL;

    void *rawcode = NULL;

    void *cstpool_start_pos, *ins_start_pos;

    unsigned long int *offsets;
    unsigned long int cstpool_start_offset, ins_start_offset;
    unsigned long int insnum;
    char *instr;

    fp = fopen(src, "r");
    if (fp != NULL) {
        rawcode = read_file_content(fp);
        if (rawcode == NULL) {
            LOG_ERROR(INTERNAL_ERROR, CANNOT_ALLOCATE_MEMORY);
            goto error;
        }
    } else {
        LOG_ERROR(INTERNAL_ERROR, FILE_DOES_NOT_EXISTS);
        goto error;
    } 

    /* load positions */
    offsets = (unsigned long int *)rawcode;

    cstpool_start_offset = offsets[0];
    ins_start_offset = offsets[1];

    /* initialize constant pool */
    cstpool_start_pos = (void *)((char *)rawcode + cstpool_start_offset);
    vm.constant_pool.count = *(unsigned long int *)cstpool_start_pos;
    vm.constant_pool.position = (unsigned long int *)cstpool_start_pos + 1;


    /* initialize instrs */
    ins_start_pos = (void *)((char *)rawcode + ins_start_offset);

    vm.instructions.length = *(unsigned long int *)ins_start_pos;
    vm.instructions.inslist = (char *)ins_start_pos + sizeof(unsigned long int);

    /* initialize program counter */
    vm.registers.pc = vm.instructions.inslist;

    return 0;

error:
    if (rawcode != NULL) {
        free(rawcode);
    }
    if (fp != NULL) {
        fclose(fp);
    }
    return -1;
}

int execute(void) {
    
    return 0;
}

void halt(struct vm *vm) {
    if (vm->rawcode != NULL) {
        free(vm->rawcode);
        vm->rawcode = NULL;
    }

    free_stack();
    free_heap();

}

void free_stack() {
    free(vm.stack.stack);
    vm.stack.capacity = vm.stack.length = 0;
}

void free_heap(){

    struct heap_item *temp = vm.heap.list;

    while (vm.heap.list != NULL) {
        vm.heap.list = vm.heap.list->next;
        free(temp);
        temp = vm.heap.list;
    }
}

int set_stack_capacity(size_t capacity) {

    return 0;
}

int set_heap_capacity(size_t capacity) {
    
    return 0;
}

void *allocate(size_t meta_size, unsigned long int refcont, size_t data_size) {
    struct heap_item *new_heap_item;

    new_heap_item = (struct heap_item *)malloc(sizeof(struct heap_item) + meta_size + data_size);

    new_heap_item->gcflag = !vm.heap.survive_flag;
    new_heap_item->meta_size = meta_size;
    new_heap_item->data_size = data_size;
    new_heap_item->next = vm.heap.list;
    vm.heap.list = new_heap_item->next;

    return (void *)new_heap_item + sizeof(new_heap_item);
}

void gc(struct heap *heap) {

}

