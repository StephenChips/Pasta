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

#define CSTPOOL_START_POS(rowcode) (rowcode)
#define INSTR_START_POS(rowcode) (rowcode + sizeof(void *))
#define INSTR_END_POS(rowcode)  (rowcode + sizeof(void *) << 1)

#define IS_STACK_ITEM(position) ((position) >= vm.stack.stack &&  (position) <= (vm.stack.stack + vm.stack.capacity))

extern struct heap;

struct conf {
    size_t stack_capacity, heap_capacity;
};

struct stack {
    size_t capacity, length;
    void *stack;
};

struct registers {
    register void *sp, *bp, *hr, *pc;
};

struct inslist {
    int length;
    char *inslist;
};

struct constant_pool {
    int count;
    void **position;
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

int halt();

int set_stack_capacity(size_t capacity);

int set_heap_capacity(size_t capacity);

#endif
