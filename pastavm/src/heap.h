#ifndef INCLUDED_HEAP_H
#define INCLUDED_HEAP_H

#define GC_DEFAULT_SURVIVE_FLAG 1
#define GC_IGNORE_FLAG -0xFFFFFFFF

typedef struct {
    int survive_flag;
    size_t current_size, capacity;
    struct heap_list *list;
} Heap;

struct heap_list {
    void *data;
    struct heap_list *next;
};

struct heap_item_info {
    char gcflag;
    unsigned int item_count;
};

void *Heap_Allocate(Heap *self, size_t data_size);

void Heap_DeleteAll(Heap *self);

void Heap_GC(Heap *self);

void *AllocateHeapItem(struct heap_item_info info);

void __Heap_MarkHeapItem(void *heap_item);

void __Heap_Sweep();

#endif /* INCLUDED_HEAP_H */
