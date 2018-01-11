#include <stdio.h>
#include <stdlib.h>
#include "cJSON.h"
#include "errlog.h"
#include "vm.h"

int load(const char *src) {

    struct conf config;
    int err;

    LOG_ERROR(NO_ERROR, OK); 

    err = __load_config(&config);    
    if (err == 0) {
        __init_heap(config.heap_capacity);
        err = __init_stack(config.stack_capacity);
        if (err == 0) {
            return __load_bytecode_file(src);
        } else {
            return -1;
        }
    } else {
        return -1;
    }
}

int __load_config(struct conf *config) {

    FILE *fp = NULL; 
    char *config_string = NULL;
    int err;

    fp = fopen(__CFNAME_CONF_FILE_NAME, "r");
    
    if (fp == NULL) {
        __create_default_config(config);
    } else {
        config_string = __read_file_content(fp);
        if (config_string == NULL) {
            goto error;
        }

        if (__get_config(config_string, config) != 0) {
            goto error;
        }

        fclose(fp);
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

int __get_config(const char *config_json, struct conf *config) {

    unsigned long heap_capacity, stack_capacity;
    cJSON *json_conf, *json_heap_capacity, *json_stack_capacity;
    json_conf = json_heap_capacity = json_stack_capacity = NULL;
    
    json_conf = cJSON_Parse(config_json);
    if (json_conf == NULL) {
        LOG_ERROR(CONFIG_ERROR, ILLEGAL_CONFIG_FILE_FORMAT);
        goto error;
    }

    json_heap_capacity  = cJSON_GetObjectItemCaseSensitive(json_conf, __CFNAME_HEAP_CAPACITY);
    json_stack_capacity = cJSON_GetObjectItemCaseSensitive(json_conf, __CFNAME_STACK_CAPACITY); 
     
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

char *__read_file_content(FILE *fp) {

    int length;
    char *buffer = NULL; 

    fseek(fp, 0, SEEK_END);
    length = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    buffer = (char *)malloc(length * sizeof(char));
   
    if (buffer != NULL) {
        fread(buffer, 1, length, fp);
    }

    return buffer;
}

int __create_default_config(struct conf *config) {

    char *rendered;
    cJSON *json_conf;
    FILE *fp = fopen(__CFNAME_CONF_FILE_NAME,"w");

    if (fp != NULL) {
        json_conf = cJSON_CreateObject();
        cJSON_AddNumberToObject(json_conf, __CFNAME_HEAP_CAPACITY, DEFAULT_HEAP_CAPACITY);
        cJSON_AddNumberToObject(json_conf, __CFNAME_STACK_CAPACITY, DEFAULT_STACK_CAPACITY);
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
 
void __init_heap(size_t size) {
    vm.heap.survive_flag = GC_DEFAULT_SURVIVE_FLAG;
    vm.heap.current_size = 0;
    vm.heap.capacity = size;
    vm.heap.list = NULL;
}

int __init_stack(size_t size) {
    vm.stack.capacity = size; 
    vm.stack.stack = malloc(size);
    if (vm.stack.stack == NULL) {
        return -1;
    }

    vm.registers.sp = vm.stack.stack;
    vm.registers.bp = vm.registers.hr = NULL;

    __PUSH_STACK_REF_COUNT(vm);
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
int __load_bytecode_file(const char *src) {
   
    FILE *fp = NULL;
    void *rawcode, *cstpool, *inslist;

    fp = fopen(src, "r");
    if (fp != NULL) {
        rawcode = __read_file_content(fp);
        if (rawcode == NULL) {
            LOG_ERROR(INTERNAL_ERROR, CANNOT_ALLOCATE_MEMORY);
            goto error;
        }
    } else {
        LOG_ERROR(INTERNAL_ERROR, FILE_DOES_NOT_EXISTS);
        goto error;\
    } 


    vm.rawcode = rawcode;

    cstpool = __RAWCODE_CSTPOOL_ADDR(rawcode);
    inslist = __RAWCODE_INSLIST_ADDR(rawcode);

    /* initialize constant pool */
    vm.constant_pool.count = __CST_COUNT(cstpool);
    vm.constant_pool.offsets = __CST_OFFSET_ARRAY(cstpool);

    vm.instructions.length = __INS_LENGTH(inslist);
    vm.instructions.inslist = __INS_ARRAY(inslist);

    /* initialize program counter */
    vm.registers.pc = __INS_ARRAY(inslist);

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

    __free_stack();
    __free_heap();

}

void __free_stack() {
    free(vm.stack.stack);
    vm.stack.capacity = 0;
}

void __free_heap(){

    struct heap_list *temp = vm.heap.list;

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

