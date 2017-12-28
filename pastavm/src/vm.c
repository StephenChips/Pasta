#include <stdio.h>
#include <stdlib.h>
#include <json/cJSON.h>
#inlude "errlog.h"
#include "vm.h"

int load(const char *src) {

    init_vm();    

    /* initialize bytecode file */
            
    return 0;
}

int read_config_file(struct conf *config) {

    FILE *fp = NULL; 
    struct conf vm_config;
    const char *rendered, *config;
    cJSON *json_conf = NULL, *json_heap_capacity = NULL, *json_stack_capacity = NULL;
    
    fp = fopen("./vmfp.conf", "r");
    if (fp == NULL) {
        fp = fopen("./vmfp.conf","w");
        if (fp != NULL) {
            json_conf = cJSON_CreateObject();
            cJSON_AddNumberToObject(json_conf, "heap_capacity", default_config.heap_capacity);
            cJSON_AddNumberToObject(json_conf, "stack_capacity", default_config.stack_capacity);
            rendered = cJSON_Print(json_conf); 
            fputs(rendered, fp);
        } else {
            LOG_ERROR(CONFIG_ERROR, CANNOT_OPEN_CONFILG_FILE);
            goto error;
        }
        fclose(fp);

        vm_conkfig = default_fp;
    } else {
        config = read_config_file(fp);
        
        json_conf = cJSON_Parse(config);
        if (json_conf == NULL) {
            LOG_ERROR(CONFIG_ERROR, ILLEGAL_CONFIG_FILE_FORMAT);
            goto error;
        }
    
        /* load configuration */
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
    
        vm_config.heap_capacity = (int)json_heap_capacity->valuedouble;
        vm_config.heap_capacity = (int)json_stack_capacity->valuedouble;
    
        if (vm_config.heap_capacity <= MIN_HEAP_CAPACITY || vm_config.heap_capacity >= MAX_HEAP_CAPACITY) {
            LOG_ERROR(CONFIG_ERROR, HEAP_CAPACITY_OUT_OF_RANGE);
            goto error;
        }
        else if (vm_config.stack_capacity <= MIN_STACK_CAPACITY || vm_config.stack_capacity >= MAX_STACK_CAPACITY) {
            LOG_ERROR(CONFIG_ERROR, STACK_CAPACITY_OUT_OF_RANGE);
            goto error;
        } 
        *config = vm_config;
        free(config);
    }

    return 0;

error:
    if (json_conf != NULL) {
       cJSON_Delete(json_conf);
    }
    if (json_heap_capacity != NULL) {
        cJSON_Delete(json_heap_capacity);
    }
    if (json_stack_capacity != NULL) {
        cJSON_Delete(json_stack_capacity);
    }

    return -1;
}

int init_heap(size_t size) {
    
    return 0;
}

int init_stack(size_t size) {
    
    return 0;
}

int init_bytecode_file(const char *src) {

    return 0;
}

int execute(void) {
    
    return 0;
}

int halt() {

    return 0;
}

int set_stack_capacity(size_t capacity) {

    return 0;
}

int set_heap_capacity(size_t capacity) {
    
    return 0;
}

