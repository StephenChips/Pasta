#ifndef VM_H_INCLUDED
#define VM_H_INCLUDED

#define CSTPOOL_INDEX_TABLE_LEN(cst_pool) (*((int *)cst_pool))
#define CSTPOOL_INDEX_TABLE(cst_pool) (cst_pool + 1)

#define DEFAULT_HEAP_CAPACITY ((unsigned long)1 << 30) /* 1 GB */ 
#define MAX_HEAP_CAPACITY     ((unsigned long)1 << 33) /* 16 GB */
#define MIN_HEAP_CAPACITY     ((unsigned long)1 << 19) /* 512 MB */

#define DEFAULT_STACK_CAPACITY ((unsigned long)1 << 22) /* 4 MB */
#define MIN_STACK_CAPACITY      ((unsigned long)1 << 21) /* 2 MB */
#define MAX_STACK_CAPACITY      ((unsigned long)1 << 19) /* 512 MB */

#define CSTPOOL_START_POS(rowcode) (rowcode)
#define INSTR_START_POS(rowcode) (rowcode + sizeof(void *))
#define INSTR_END_POS(rowcode)  (rowcode + sizeof(void *) << 1)

#define IS_STACK_ITEM(position) ((position) >= vm.stack.stack &&  (position) <= (vm.stack.stack + vm.stack.capacity))

#define DEFAULT_SURVIVE_FLAG 0

#define HEAP_META(h)  ((struct heap_item *)h)
#define META_SIZE(h)  (((struct heap_item *)h)->meta_size)
#define DATA_SIZE(h)  (((struct heap_item *)h)->data_size)
#define META_START(h) (h + sizeof(struct heap_item))
#define DATA_START(h) (h + sizeof(struct heap_item) + (META_SIZE(h)))
#define HEAP_END(h)   (h + sizeof(struct heap_item) + (META_SIZE(h)) + (DATA_SIZE(h)))

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
    size_t current_size, max_size;
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
    size_t capacity, length;
    void *stack;
};

struct registers {
    char *sp, *bp, *hr, *pc;
};

struct inslist {
    int length;
    char *inslist;
};

struct constant_pool {
    int count;
    unsigned long int *position;
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

#endif
