#include <stdio.h>
#include <stdlib.h>
#include "cJSON.h"
#include "errlog.h"
#include "rawcode.h"
#include "positions.h"
#include "vm.h"

#define __ABS(_a) ((_a) < 0 ? - (_a) : (_a))

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
    vm.stack.stack = (item_t *)malloc(size);
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
int __load_bytecode_file(const char *src) {
   
    FILE *fp = NULL;
    void *rawcode, *cstpool, *inslist;

    fp = fopen(src, "rb");
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
    
    struct ins ins;

    long long int itmp1, itmp2;
    double ftmp1, ftmp2;
    item_t *rtmp1, *rtmp2;
    void *mtmp1;

    int i;

    while (vm.registers.pc - vm.instructions.inslist < vm.instructions.length) {
        switch(*(vm.registers.pc)) {
        case ICONST:
            __ITEM_SET_INT(vm.registers.sp, __INS_ICONST_VALUE(vm.registers.pc));
            vm.registers.sp++;
            vm.registers.pc += INS_ICONST_SIZE;
            break;
        case FCONST:
            __ITEM_SET_FLOAT(vm.registers.sp, __INS_FCONST_VALUE(vm.registers.pc));
            vm.registers.sp++;
            vm.registers.pc += INS_FCONST_SIZE;
            break;
        case CCONST:
            __ITEM_SET_CHAR(vm.registers.sp, __INS_CCONST_VALUE(vm.registers.pc));
            vm.registers.sp++;
            vm.registers.pc += INS_CCONST_SIZE;
            break;
        case IADD:
            itmp1 = __ITEM_GET_INT(--vm.registers.sp);
            itmp2 = __ITEM_GET_INT(--vm.registers.sp);
            __ITEM_SET_INT(vm.registers.sp, itmp1 + itmp2);
            vm.registers.pc += INS_IADD_SIZE;
            vm.registers.sp++;
            break;
        case ISUB:
            itmp1 = __ITEM_GET_INT(--vm.registers.sp);
            itmp2 = __ITEM_GET_INT(--vm.registers.sp);
            __ITEM_SET_INT(vm.registers.sp, itmp1 - itmp2);
            vm.registers.pc += INS_ISUB_SIZE;
            vm.registers.sp++;
            break;
        case FADD:
            ftmp1 = __ITEM_GET_FLOAT(--vm.registers.sp);
            ftmp2 = __ITEM_GET_FLOAT(--vm.registers.sp);
            __ITEM_SET_FLOAT(vm.registers.sp, ftmp1 + ftmp2);
            vm.registers.pc += INS_FADD_SIZE;
            vm.registers.sp++;
            break;
        case FSUB:
            ftmp1 = __ITEM_GET_FLOAT(--vm.registers.sp);
            ftmp2 = __ITEM_GET_FLOAT(--vm.registers.sp);
            __ITEM_SET_FLOAT(vm.registers.sp, ftmp1 - ftmp2);
            vm.registers.pc += INS_FSUB_SIZE;
            vm.registers.sp++;
            break;
        case IMUL:
            itmp1 = __ITEM_GET_INT(--vm.registers.sp);
            itmp2 = __ITEM_GET_INT(--vm.registers.sp);
            __ITEM_SET_INT(vm.registers.sp, itmp1 * itmp2);
            vm.registers.pc += INS_IMUL_SIZE;
            vm.registers.sp++;
            break;
        case FMUL:
            ftmp1 = __ITEM_GET_FLOAT(--vm.registers.sp);
            ftmp2 = __ITEM_GET_FLOAT(--vm.registers.sp);
            __ITEM_SET_FLOAT(vm.registers.sp, ftmp1 * ftmp2);
            vm.registers.pc += INS_FSUB_SIZE;
            vm.registers.sp++;
            break;
        case IDIV:
            itmp1 = __ITEM_GET_INT(--vm.registers.sp);
            itmp2 = __ITEM_GET_INT(--vm.registers.sp);

            if (itmp1 == 0) {
                error_logger.err = INTERNAL_ERROR;
                error_logger.msg = DIVIDED_BY_ZERO;
            }

            __ITEM_SET_INT(vm.registers.sp, itmp1 / itmp2);
            vm.registers.pc += INS_ISUB_SIZE;
            vm.registers.sp++;
            break;
        case FDIV:
            ftmp1 = __ITEM_GET_FLOAT(--vm.registers.sp);
            ftmp2 = __ITEM_GET_FLOAT(--vm.registers.sp);
            __ITEM_SET_FLOAT(vm.registers.sp, ftmp1 / ftmp2);
            vm.registers.pc += INS_FSUB_SIZE;
            vm.registers.sp++;
            break;
        case MOD:
            itmp1 = __ITEM_GET_INT(--vm.registers.sp);
            itmp2 = __ITEM_GET_INT(--vm.registers.sp);
            __ITEM_SET_INT(vm.registers.sp, itmp1 % itmp2);
            vm.registers.pc += INS_MOD_SIZE;
            vm.registers.sp++;
            break;
        case IEQ:
            itmp1 = __ITEM_GET_INT(--vm.registers.sp);
            itmp2 = __ITEM_GET_INT(--vm.registers.sp);
            __ITEM_SET_INT(vm.registers.sp, itmp1 == itmp2);
            vm.registers.pc += INS_IEQ_SIZE;
            vm.registers.sp++;
            break;
        case FEQ:
            ftmp1 = __ITEM_GET_FLOAT(--vm.registers.sp);
            ftmp2 = __ITEM_GET_FLOAT(--vm.registers.sp);
            __ITEM_SET_INT(vm.registers.sp, ftmp1 == ftmp2);
            vm.registers.pc += INS_FEQ_SIZE;
            vm.registers.sp++;
            break;
        case CEQ:
            itmp1 = __ITEM_GET_CHAR(--vm.registers.sp);
            itmp2 = __ITEM_GET_CHAR(--vm.registers.sp);
            __ITEM_SET_INT(vm.registers.sp, itmp1 == itmp2);
            vm.registers.pc += INS_CEQ_SIZE;
            vm.registers.sp++;
            break;
        case REQ:
            rtmp1 = __ITEM_GET_REF(--vm.registers.sp);
            rtmp2 = __ITEM_GET_REF(--vm.registers.sp);
            __ITEM_SET_INT(vm.registers.sp, itmp1 == itmp2);
            vm.registers.pc += INS_REQ_SIZE;
            vm.registers.sp++;
            break;
        case ILT:
            itmp1 = __ITEM_GET_INT(--vm.registers.sp);
            itmp2 = __ITEM_GET_INT(--vm.registers.sp);
            __ITEM_SET_INT(vm.registers.sp, itmp1 < itmp2);
            vm.registers.pc += INS_ILT_SIZE;
            vm.registers.sp++;
            break;
        case FLT:
            ftmp1 = __ITEM_GET_FLOAT(--vm.registers.sp);
            ftmp2 = __ITEM_GET_FLOAT(--vm.registers.sp);
            __ITEM_SET_INT(vm.registers.sp, ftmp1 < ftmp2);
            vm.registers.pc += INS_FLT_SIZE;
            vm.registers.sp++;
            break;
        case CLT:
            itmp1 = __ITEM_GET_CHAR(--vm.registers.sp);
            itmp2 = __ITEM_GET_CHAR(--vm.registers.sp);
            __ITEM_SET_INT(vm.registers.sp, itmp1 < itmp2);
            vm.registers.pc += INS_CLT_SIZE;
            vm.registers.sp++;
            break;
        case RLT:
            rtmp1 = __ITEM_GET_REF(--vm.registers.sp);
            rtmp2 = __ITEM_GET_REF(--vm.registers.sp);
            __ITEM_SET_INT(vm.registers.sp, itmp1 < itmp2);
            vm.registers.pc += INS_RLT_SIZE;
            vm.registers.sp++;
            break;
        case NOT:
            itmp1 = __ITEM_GET_INT(--vm.registers.sp);
            __ITEM_SET_INT(vm.registers.sp, !itmp1);
            vm.registers.pc += INS_NOT_SIZE;
            vm.registers.sp++;
            break;
        case DUP:
            *vm.registers.sp = *(vm.registers.sp - 1);
            vm.registers.pc += INS_DUP_SIZE;
            vm.registers.sp++;
            break;
        case LOAD:
            itmp1 = __ITEM_GET_INT(--vm.registers.sp);
            if (itmp1 < 0 || itmp1 >= vm.registers.sp - vm.stack.stack) { 
                error_logger.err = INTERNAL_ERROR;
                error_logger.msg = ILLEGAL_INDEX; 
                return -1;
            }
            *vm.registers.sp = vm.stack.stack[itmp1];
            vm.registers.pc += INS_LOAD_SIZE;
            vm.registers.sp++;
            break;
        case STORE:
            itmp1 = *(--vm.registers.sp);
            itmp2 = __ITEM_GET_INT(--vm.registers.sp);
            if (itmp2 < 0 || itmp2 >= vm.registers.sp - vm.stack.stack) {
                error_logger.err = INTERNAL_ERROR;
                error_logger.msg = ILLEGAL_INDEX; 
                return -1;
            }
            vm.stack.stack[itmp2] = itmp1; 
            vm.registers.pc += INS_STORE_SIZE;
            vm.registers.sp++;
            break;
        case GETSP:
            __ITEM_SET_INT(vm.registers.sp, (int)(long long int)vm.registers.sp);
           vm.registers.sp++;
           vm.registers.pc += INS_GETSP_SIZE;
           break;
        case GETBP:
            __ITEM_SET_INT(vm.registers.sp, (int)(long long int)vm.registers.bp);
           vm.registers.sp++;
           vm.registers.pc += INS_GETBP_SIZE;
           break;
        case ALTSP:
           itmp1 = *(signed long int *)(vm.registers.pc + sizeof(char));
           if (__ABS(itmp1) > __CURRENT_ITEM_COUNT(vm)) {
               error_logger.err = INTERNAL_ERROR;
               error_logger.msg = ILLEGAL_ALTSP_ARGS;
               return -1;
           }
           vm.registers.sp += *(signed long int *)(vm.registers.pc + sizeof(char));
           vm.registers.pc += INS_ALTSP_SIZE;
           break;
        case JUMP:
            itmp1 = *(unsigned long int *)(vm.registers.pc + sizeof(char));
            if (itmp1 < 0 || itmp1 >= vm.instructions.length) {
                error_logger.err = INTERNAL_ERROR;
                error_logger.msg = ILLEGAL_JUMPING_ADDR;
                return -1;
            }
            vm.registers.pc = vm.instructions.inslist + itmp1;
            break;
        case JPZ:
            itmp1 = *(unsigned long int *)(vm.registers.pc + sizeof(char));
            itmp2 = __ITEM_GET_INT(--vm.registers.sp); 

            if (itmp1 < 0 || itmp1 >= vm.instructions.length) {
                error_logger.err = INTERNAL_ERROR;
                error_logger.msg = ILLEGAL_JUMPING_ADDR;
                return -1;
            }

            if (itmp1 == 0) {
                vm.registers.pc = vm.instructions.inslist + itmp1;
            }
            break;
        case JPNZ:
            itmp1 = *(unsigned long int *)(vm.registers.pc + sizeof(char));
            itmp2 = __ITEM_GET_INT(--vm.registers.sp); 

            if (itmp1 < 0 || itmp1 >= vm.instructions.length) {
                error_logger.err = INTERNAL_ERROR;
                error_logger.msg = ILLEGAL_JUMPING_ADDR;
                return -1;
            }

            if (itmp1 != 0) {
                vm.registers.pc = vm.instructions.inslist + itmp1;
            }
            break;
        case CALL:
            itmp1 = *(unsigned int *)(vm.registers.pc + sizeof(char));
            itmp2 = *(unsigned long int *)(vm.registers.pc + sizeof(char) + sizeof(unsigned int));

            /* if number of argument negetive */
            if (itmp1 < 0) {
                error_logger.err = INTERNAL_ERROR;
                error_logger.msg = ILLEGAL_ARG_COUNT;
                return -1;
            }

            /* if function start offset out of range */
            if (itmp2 < 0 || itmp2 >= vm.instructions.length) {
                error_logger.err = INTERNAL_ERROR;
                error_logger.msg = ILLEGAL_JUMPING_ADDR;
                return -1;
            }

            /* move up items with 2 offset, n equals the number of function's argument */
            for (i = 0; i < itmp1; i++) {
                vm.registers.sp--;
                *(vm.registers.sp + 2) = *vm.registers.sp;
            }

            /* store offsets instead of address */
            __ITEM_SET_INT(vm.registers.sp, (int)(vm.registers.pc - vm.instructions.inslist + INS_CALL_SIZE)); /* return address */
            __ITEM_SET_INT(vm.registers.sp + 1, (int)(vm.registers.bp - vm.stack.stack));      /* old stack frame base pointer */

            vm.registers.pc = vm.instructions.inslist + itmp2;
            vm.registers.bp = vm.registers.sp;
            vm.registers.sp += 2;
            break;

        case TCALL:
            itmp1 = *(unsigned int *)(vm.registers.pc + sizeof(char));
            itmp2 = *(unsigned long int *)(vm.registers.pc + sizeof(char) + sizeof(unsigned int));

            /* if number of argument negetive */
            if (itmp1 < 0) {
                error_logger.err = INTERNAL_ERROR;
                error_logger.msg = ILLEGAL_ARG_COUNT;
                return -1;
            }

            /* if function start offset out of range */
            if (itmp2 < 0 || itmp2 >= vm.instructions.length) {
                error_logger.err = INTERNAL_ERROR;
                error_logger.msg = ILLEGAL_JUMPING_ADDR;
                return -1;
            }

            for (i = itmp1 - 1; i >= 0; i--) {
                vm.registers.sp--;
                (vm.registers.bp + 2)[i] = *vm.registers.sp;
            }

            vm.registers.sp = vm.registers.bp + 2;
            vm.registers.pc = vm.instructions.inslist + itmp2;

            break;
        case RETURN:
            /* set program counter to the old context */
            vm.registers.pc = vm.instructions.inslist + __STKFRAME_RETADDR(vm.registers.bp);

            /* assign res to ret */
            *vm.registers.bp = *(vm.registers.sp - 1);

            /* delete current stack frame */
            vm.registers.sp = vm.registers.bp + 1;

            /* resume the old stack frame */
            vm.registers.bp = vm.stack.stack + *(vm.registers.bp + 1);
            break;
        case STOP:
            Heap_DeleteAll(&(vm.heap));
            vm.registers.sp = vm.stack.stack;
            vm.registers.pc = vm.instructions.inslist;
            vm.registers.bp = vm.registers.hr = NULL; 
            return 0;
        case RAISE:
            /* raise exception if no handler is set */
            if (vm.registers.hr == NULL) {
                error_logger.err = EXCEPTION_RAISED;
                return -1;
            }

            /* do loop and match exception id with exception id giving by instruction */
            while (1) {
                 itmp1 = *(unsigned int *)(vm.registers.pc + sizeof(char));

                 /* matched, then jump to correspondence address */
                 if (__EXNHDR_EXNID(vm.registers.hr) == itmp1) {
                     vm.registers.pc = vm.instructions.inslist + __EXNHDR_JUMPOFFSET(vm.registers.hr);
                     vm.registers.sp = vm.registers.hr;
                     vm.registers.hr = (__EXNHDR_OUTERHDR(vm.registers.hr) < 0) ? NULL : (vm.stack.stack + __EXNHDR_OUTERHDR(vm.registers.hr));

                     break;
                 } 
                 /* unmatched, but has outer excepiton handlers */
                 else if (__EXNHDR_OUTERHDR(vm.registers.hr) >= 0) {
                     vm.registers.hr = vm.stack.stack + __EXNHDR_OUTERHDR(vm.registers.hr);
                 }
                 /* unmatched, no more outer exception handers */
                 else {
                     error_logger.err = EXCEPTION_RAISED;
                     return -1;
                 }
             } 

             break;
        case PUSHEXN:
            itmp1 = *(unsigned int *)(vm.registers.pc + sizeof(char));
            itmp2 = *(unsigned long int *)(vm.registers.pc + sizeof(char) + sizeof(unsigned int));

            __ITEM_SET_INT(vm.registers.sp, itmp1); /* store exception id */
            vm.registers.sp++;
            __ITEM_SET_INT(vm.registers.sp, itmp2); /* store jumping offset */
            vm.registers.sp++;

            /* store current exception handler's offset to stack */
            if (vm.registers.hr == NULL) {
                __ITEM_SET_INT(vm.registers.sp, -999);    
            } else {
                __ITEM_SET_INT(vm.registers.sp, vm.registers.hr - vm.stack.stack);
            }
            vm.registers.sp++;
            vm.registers.pc += INS_PUSHEXN_SIZE;
            break;
        case POPEXN:
            if (vm.registers.hr == NULL) {
                error_logger.err = INTERNAL_ERROR;
                error_logger.msg = NO_EXN_HDR;
                return -1;
            }

            vm.registers.sp = vm.registers.hr;
            (__EXNHDR_OUTERHDR(vm.registers.hr) <= 0) ? NULL : ((vm.stack.stack) + __EXNHDR_OUTERHDR(vm.registers.hr));
            vm.registers.pc += INS_POPEXN_SIZE;
            break;
        case NEW:
            itmp1 = *(unsigned long int *)(vm.registers.pc + sizeof(char));
            rtmp1 = Heap_Allocate(&(vm.heap), itmp1);
            __ITEM_SET_REF(vm.registers.sp, rtmp1);
            vm.registers.sp++;
            vm.registers.pc += INS_NEW_SIZE;
            break;
        case GETDATA:
            itmp1 = __ITEM_GET_INT(--vm.registers.sp);
            mtmp1 = __ITEM_GET_REF(--vm.registers.sp);

            *(vm.registers.sp) = __HEAPITEM_GETITEM(mtmp1, itmp1);
            vm.registers.sp++;
            vm.registers.pc += INS_GETDATA_SIZE; 
            break;
        case SETDATA:
            itmp1 = *(--vm.registers.sp);
            itmp2 = __ITEM_GET_INT(--vm.registers.sp);
            mtmp1 = __ITEM_GET_REF(--vm.registers.sp);

            __HEAPITEM_GETITEM(mtmp1, itmp2) = itmp1;
            vm.registers.pc += INS_SETDATA_SIZE;
            break;
        case I2F:
            ftmp1 = __ITEM_GET_INT(--vm.registers.sp);
            __ITEM_SET_INT(vm.registers.sp, ftmp1);

            vm.registers.sp++;
            vm.registers.pc += INS_I2F_SIZE;
            break;
        case F2I:
            itmp1 = (int)__ITEM_GET_FLOAT(--vm.registers.sp);
            __ITEM_SET_INT(vm.registers.sp, itmp1);

            vm.registers.sp++;
            vm.registers.pc += INS_F2I_SIZE;
            break;
        case C2I:
            itmp1 = __ITEM_GET_CHAR(--vm.registers.sp);

            __ITEM_SET_INT(vm.registers.sp, itmp1);
            vm.registers.sp++;
            vm.registers.pc += INS_C2I_SIZE;
            break;
        case I2C:
            itmp1 = (char)__ITEM_GET_INT(--vm.registers.sp);
            __ITEM_SET_CHAR(vm.registers.sp, itmp1);

            vm.registers.sp++;
            vm.registers.pc += INS_C2I_SIZE;
            break;
        case LDC:
            itmp1 = *(unsigned int *)(vm.registers.pc + sizeof(char));

            if (itmp1 >= vm.constant_pool.count) {
                error_logger.err = INTERNAL_ERROR;
                error_logger.msg = IDX_OUT_OF_RANGE;
                return -1;
            }
            rtmp1 = (item_t *)(vm.constant_pool.offsets[itmp1] + __HEAPITEM_INFO_SIZE);

            __ITEM_SET_REF(vm.registers.sp, rtmp1);
            vm.registers.sp++;
            vm.registers.pc += INS_LDC_SIZE;
            break;
        default:
            LOG_ERROR(INTERNAL_ERROR, UNKNOWN_INS);
            return -1;
        }
    }

    return 0;
}

void halt() {
    if (vm.rawcode != NULL) {
        free(vm.rawcode);
        vm.rawcode = NULL;
    }

    __free_stack();
    __free_heap();

}

void __free_stack() {
    free(vm.stack.stack);
    vm.stack.capacity = 0;
}

void __free_heap(){

    Heap_DeleteAll(&(vm.heap));
}

int set_stack_capacity(size_t capacity) {

    return 0;
}
    
int set_heap_capacity(size_t capacity) {

    return 0;
}

