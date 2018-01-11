#ifndef VM_H_INCLUDED
#define VM_H_INCLUDED

/* using:
 * type:
 *    FILE
 */
#include <stdio.h>
#include "positions.h"
#include "heap.h"

#define CSTPOOL_INDEX_TABLE_LEN(cst_pool) (*((int *)cst_pool))
#define CSTPOOL_INDEX_TABLE(cst_pool) (cst_pool + 1)

#define DEFAULT_HEAP_CAPACITY ((unsigned long)1 << 30) /* 1 GB */ 
#define MAX_HEAP_CAPACITY     ((unsigned long)1 << 33) /* 16 GB */
#define MIN_HEAP_CAPACITY     ((unsigned long)1 << 19) /* 512 MB */

#define DEFAULT_STACK_CAPACITY ((unsigned long)1 << 22) /* 4 MB */
#define MIN_STACK_CAPACITY      ((unsigned long)1 << 19) /* 512 KB */
#define MAX_STACK_CAPACITY      ((unsigned long)1 << 26) /* 64 MB */

#define OPEN_VAL 1
#define CLOSE_VAL 0
#define __OPEN_RR(vm) ((_vm).registers.rr = RR_OPEN_VAL)
#define __CLOSE_RR(vm) ((_vm).registers.rr = RR_CLOSE_VAL)
#define __IS_RR_OPEN(vm) ((_vm).registers.rr == RR_OPEN_VAL)

#define INIT_STK_REF_COUNT 0
#define __CFNAME_HEAP_CAPACITY "HeapCapacity"
#define __CFNAME_STACK_CAPACITY "StackCapacity"
#define __CFNAME_CONF_FILE_NAME "vm.conf"

#define __STACK_REF_COUNT(_stack) \
    ((vm.registers.bp < 0) ? (*(int *)vm.stack.stack) : (*(int *)vm.registers.bp))

#define __PUSH_STACK_REF_COUNT(_vm) __PUSH_INT(_vm, INIT_STK_REF_COUNT)

/* ABOUT RR RIG:
 * ONLY WHEN */

/* need to be redefine */
#if 0
#define __PUSH_INT(_vm, _val) \
do {\
    *((int *)(_vm).registers.sp) = (_val);\
    (_vm).registers.sp += sizeof(int);\
} while (0)

#define __PUSH_DOUBLE(_vm, _val)\
do {\
    *((double *)(_vm).registers.sp) = (_val);\
    (_vm).registers.sp += sizeof(double);\
} while (0)
   
#define __PUSH_CHAR(_vm, _val)\
do {\
    *((char *)(_vm).registers.sp) = (_val);\
    (_vm).registers.sp += sizeof(char);\
} while (0)

#define __PUSH_REF(_vm, _val)\
do {\
    *((void **)(_vm).registers.sp) = (_val);\
    (_vm).registers.sp += sizeof(void *);\
} while (0)

#define __POP_INT(_vm) ((_vm).registers.sp -= sizeof(int))
#define __POP_DOUBLE(_vm) ((_vm).registers.sp -= sizeof(double))
#define __POP_CHAR(_vm) ((_vm).registers.sp -= sizeof(char)
#define __POP_REF(_vm) ((_vm).registers.sp -= sizeof(void *))
#endif


/* ITEMS IN A CONSTANTPOOL WOULD BE
 * 
 * 1. integer, float, char
 * 2. compound data type like array or record, with the form of heap item.
 */

struct conf {
    unsigned long int stack_capacity, heap_capacity;
};

struct stack {
    size_t capacity;
    char *stack;
};

struct registers {
    char *sp, *bp, *hr, *pc, rr;
};

struct inslist {
    unsigned long int length;
    char *inslist;
};

struct constant_pool {
    unsigned long int count;
    unsigned long int *offsets;
};

struct vm {
    void *rawcode;
    struct stack stack;
    struct registers registers;
    struct constant_pool constant_pool;
    Heap heap;
    struct inslist instructions;
};

struct vm vm;

int load(const char *src);

int execute();

void halt();

int set_stack_capacity(size_t capacity);

int set_heap_capacity(size_t capacity);
/* PRIVATE FUNCTIONS, DO NOT CALL IT FOR ORDINARY USE */
int  __load_config(struct conf *config);
int  __get_config(const char *config_json, struct conf *config);
int  __create_default_config(struct conf *config);
char *__read_file_content(FILE *fp);
void __init_heap(size_t size);
int  __init_stack(size_t size);
int  __load_bytecode_file(const char *src);
void __free_stack();
void __free_heap();

#endif
