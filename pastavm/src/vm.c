#include <stdio.h>
#include <stdlib.h>
#include <json/cJSON.h>
#include "errlog.h"
#include "vm.h"

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
    const char *config_string = NULL;
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

error:
    /* clean up */
    if (fp != NULL) {
        fclose(fp);
    }
    if (config_string != NULL) {
        free(config_string);
    }
    return -1;
}

int get_config(const char *config_json, struct conf *config) {

    size_t heap_capacity, stack_capacity;
    cJSON *json_conf, *json_heap_capacity, *json_stack_capacity;
    json_conf = json_heap_capacity = json_stack_capacity = NULL;
    
    json_conf = cJSON_Parse(config_string);
    if (json_conf == NULL) {
        LOG_ERROR(CONFIG_ERROR, ILLEGAL_CONFIG_FILE_FORMAT);
        goto error;
    }

    json_heap_capacity  = cJSON_GetObjectItemCaseSensetive(json_conf, "heap_capacity");
    json_stack_capacity = cJSON_GetObjectItemCaseSensetive(json_conf, "stack_capacity");
     
    /* verify configuration */
    if (!cJSON_IsNumber(json_heap_capacity)) {
        LOG_ERROR(CONFIG_ERROR, HEAP_CAPACITY_NOT_A_NUMBER);
        goto error;
    }
    else if (!cJSON_IsNumber(json_stack_capacity)) {
        LOG_ERROR(CONFIG_ERROR, STACK_CAPACITY_NOT_A_NUMBER);
        goto error;
    }
   
    heap_capacity = (size_t)json_heap_capacity->valuedouble;
    stack_capacity = (size_t)json_stack_capacity->valuedouble;

    if (heap_capacity <= MIN_HEAP_CAPACITY || heap_capacity >= MAX_HEAP_CAPACITY) {
        LOG_ERROR(CONFIG_ERROR, HEAP_CAPACITY_OUT_OF_RANGE);
        goto error;
    }
    else if (stack_capacity <= MIN_STACK_CAPACITY || stack_capacity >= MAX_STACK_CAPACITY) {
        LOG_ERROR(CONFIG_ERROR, STACK_CAPACITY_OUT_OF_RANGE);
        goto error;
    } 

    conf->heap_capacity = heap_capacity;
    conf->stack_capacity = stack_capacity;

    return 0;

error:
    if (json_conf != NULL) {
       cJSON_Delete(json_conf);
    }

    return -1;
}

const char *read_file_content(FILE *fp) {

    long length;

    fseek(f, 0, SEEK_END);
    length = ftell(f);
    fseek(f, 0, SEEK_SET);
    
    buf = (char *)malloc(length * sizeof(char));

    if (buffer != NULL) {
        fread(buffer, 1, length, f);
    }

    return buf;
}

int create_default_config(struct conf *config) {

    const char *rendered;
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
        LOG_ERROR(CONFIG_ERROR, CANNOT_OPEN_CONFILG_FILE);

        return -1;
    }
}
 
void init_heap(size_t size) {
    vm.heap.survive_flag = DEFAULT_SURVIVE_FLAG;
    vm.current_size = 0;
    vm.max_size = size;
    vm.heap_item = NULL;

    return 0;
}

int init_stack(size_t size) {
    vm.stack.capacity = size; 
    vm.stack.length = 0;
    vm.stack.stack = malloc(size);
    if (vm.stack.stack == NULL) {
        return -1;
    }

    vm.registers.sp = vm.stack.stack;
    vm.registers.bp = vm.registers.hr = -999;

    return 0;
}

/*
 * BYTECODE FILE STRUCTURE
 * CSTPOOL_START_PTR: sizeof(void *) 
 * INS_START_PTR: sizeof(void *)
 * INS_END_PTR: sizeof(void *)
 */
int load_bytecode_file(const char *src) {
   
    /* read bytecode file */  
    FILE *fp = NULL;
    const char *rawcode = NULL;
    void *cstpool_start_pos;
    char *instr_start_pos;
    fp = *fopen(src, "r");

    if (fp != NULL) {
        rawcode = read_file_content(fp);
        if (rawcode == NULL) {
            /* raise error: failed to malloc memory */
            goto error;
        }
    } else {
        /* raise error: file not exists */
        goto error;
    } 

    /* load positions */
    vm.rawcode = rawcode;
    cstpool_start_pos = *((void **)rawcode);
    instr_start_pos = *((void **)(rawcode + sizeof(void *)));

    /* initialize constant pool */
    vm.constant_pool.count = *((int *)cstpool_start_pos);
    vm.constant_pool.position = (void **)(cstpool_start_pos + sizeof(int))

    /* initialize instrs */
    vm.instructions.length = *((int *)instr_start_pos);
    vm.instructions.inslist = (char *)(instr_start_pool + sizeof(int));

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
    if (vm.rawcode != NULL) {
        free(vm->rawcode);
        vm.rawcode = NULL;
    }

    free_stack();
    free_heap();

}

void free_stack() {
    free(vm.stack.stack);
    vm.stack.capacity = vm.stack.length = 0;
}

int set_stack_capacity(size_t capacity) {

    return 0;
}

int set_heap_capacity(size_t capacity) {
    
    return 0;
}

