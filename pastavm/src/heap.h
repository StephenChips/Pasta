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
    unsigned int refcnt;
    size_t dtsz;
};

void *Heap_Allocate(Heap *self, unsigned long int ref_count, size_t data_size);

void Heap_Gc(Heap *self);

void *AllocateHeapItem(struct heap_item_info info);

#endif /* INCLUDED_HEAP_H */
