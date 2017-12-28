#ifndef VM_H_INCLUDED
#define VM_H_INCLUDED

#define CSTPOOL_INDEX_TABLE_LEN(cst_pool) (*((int *)cst_pool))
#define CSTPOOL_INDEX_TABLE(cst_pool) (cst_pool + 1)

#define DEFAULT_HEAP_CAPACITY (1 << 30) /* 1 GB */ 
#define MAX_HEAP_CAPACITY     (1 << 33) /* 16 GB */
#define MIN_HEAP_CAPACITY     (1 << 19) /* 512 MB */

#define DEFAULT_STACK_CAPACITY (1 << 22) /* 4 MB */
#define MIN_HEAP_CAPACITY      (1 << 21) /* 2 MB */
#define MAX_HEAP_CAPACITY      (1 << 19) /* 512 MB */

struct conf {
    int stack_capacity, heap_capacity;
};

struct stack {
    int capacity, length;
    void *stack;
};

struct registers {
    int sp, bp, hr, pc;
};

const struct conf default_config = {
    DEFAULT_STACK_CAPACITY,
    DEFAULT_HEAP_CAPACITY
};

void *constant_pool;

char *intructions;

int load(const char *src);

int execute();

int halt();

int set_stack_capacity(size_t capacity);

int set_heap_capacity(size_t capacity);

#endif
