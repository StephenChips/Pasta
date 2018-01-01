#ifndef VM_H_INCLUDED
#define VM_H_INCLUDED

/* using:
 * type:
 *    FILE
 */
#include <stdio.h>

#define CSTPOOL_INDEX_TABLE_LEN(cst_pool) (*((int *)cst_pool))
#define CSTPOOL_INDEX_TABLE(cst_pool) (cst_pool + 1)

#define DEFAULT_HEAP_CAPACITY ((unsigned long)1 << 30) /* 1 GB */ 
#define MAX_HEAP_CAPACITY     ((unsigned long)1 << 33) /* 16 GB */
#define MIN_HEAP_CAPACITY     ((unsigned long)1 << 19) /* 512 MB */

#define DEFAULT_STACK_CAPACITY ((unsigned long)1 << 22) /* 4 MB */
#define MIN_STACK_CAPACITY      ((unsigned long)1 << 21) /* 2 MB */
#define MAX_STACK_CAPACITY      ((unsigned long)1 << 19) /* 512 MB */

/* functions for rawcode location */
#define __RAWCODE_INDEX(rawcode)  ((void **)rawcode)
#define __RAWCODE_INDEX_SIZE (2 * sizeof (void *))
#define __CSTPOOL_START_POS(rawcode) (__RAWCODE_INDEX(rawcode)[0])
#define __INSTR_START_POS(rawcode) (__RAWCODE_INDEX(rawcode)[1])
#define __CST_COUNT(rawcode) *((unsigned long int *)CSTPOOL_START_POS(rowcode))
#define __ARRAY_OF_PTR_TO_CONSTANTS(rawcode) \
    ((char *)CSTPOOL_START_POS(rawcode) + sizeof(unsigned long int))
#define __ACCESS_CONSTANT(rawcode, index) (*ARRAY_OF_PTR_OF_CONSTANTS(rawcode)[index])
#define __INS_LENGTH(rawcode) *((unsigned long int *)INSTR_START_POS(rawcode))
#define __ARRAY_OF_INSTRUCTION(rawcode) ((char *)INSTR_START_POS(rawcode) + sizeof(unsigned long int))

#define IS_STACK_ITEM(position) (position >= vm.stack.stack &&  (position) <= vm.stack.stack + vm.stack.capacity)

#define DEFAULT_SURVIVE_FLAG 0

/* heap location */
#define __HEAP_INFO(h)  ((struct heap_item *)h)
#define __HEAP_INFO_SIZE (sizeof(struct heap_item))
#define __HEAP_REF_SIZE (sizeof(void *))
#define __HEAP_META(h) ((char *)h + HEAP_INFO_SIZE)
#define __HEAP_REF(h) ((char *)h + HEAP_INFO_SIZE + HEAP_INFO(h)->meta_size)
#define __HEAP_DATA(h) (h + HEAP_INFO_SIZE + HEAP_INFO(h)->meta_size + (HEAP_INFO(h)->ref_count)*HEAP_REF_SIZE )
#define __HEAP_END(h)   (h + HEAP_INFO_SIZE + HEAP_INFO(h)->meta_size + (HEAP_INFO(h)->ref_count)*HEAP_REF_SIZE + HEAP_INFO(h)->data_size)

#define __CMP_STKLEN_AND_OPRLEN(oprlen) /* TODO */
#define __CMP_RETLEN_AND_AVAILABLE_STKSZ(retlen) /* TODO */
#define __IS_REF_INSIDE_INSAREA(ref) /* TODO */
#define __IS_REF_INSIDE_HEAPAREA(ref) /* TODO */
#define __IS_STK_FREAM_EXISTS() /* TODO */
#define __IS_EXN_HDR_EXISTS /* TODO */
#define __IS_OFFSET_OUT_OF_META_AREA(offset) /* TODO */
#define __IS_OFFSET_OUT_OF_DATA_AREA(offset) /* TODO */
#define __IS_IDX_OUT_OF_CSTPOOL(idx) /* TODO */
#define __IS_IDX_OUT_OF_REF_AREA(idx) /* TODO */

struct heap {
    int survive_flag;
    size_t current_size, capacity;
    struct heap_item *list;
};

struct heap_item {
    char gcflag;
    int meta_size,  data_size;
    struct heap_item *next;
};


struct conf {
    unsigned long stack_capacity, heap_capacity;
};

struct stack {
    size_t capacity;
    char *stack;
};

struct registers {
    char *sp, *bp, *hr, *pc;
};

struct inslist {
    unsigned long int length;
    char *inslist;
};

struct constant_pool {
    unsigned long int count;
    void **positions;
};

struct vm {
    void *rawcode;
    struct stack stack;
    struct registers registers;
    struct constant_pool constant_pool;
    struct heap heap;
    struct inslist instructions;
};

const struct conf default_config = {
    DEFAULT_STACK_CAPACITY,
    DEFAULT_HEAP_CAPACITY
};

struct vm vm;

int load(const char *src);

int execute();

void halt();

int set_stack_capacity(size_t capacity);

int set_heap_capacity(size_t capacity);

void *allocate(size_t meta_size, unsigned long int ref_count, size_t data_size);

void gc(struct heap *heap);

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
