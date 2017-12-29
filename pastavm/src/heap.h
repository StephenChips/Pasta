#ifndef HEAP_H_INCLUDED
#define HEAP_H_INCLUDED

#define DEFAULT_SURVIVE_FLAG 0

#define HEAP_META(h)  ((struct heap_item *)h)
#define META_SIZE(h)  (((struct heap_item *)h)->meta_size)
#define DATA_SIZE(h)  (((struct heap_item *)h)->data_size)
#define META_START(h) (h + sizeof(struct heap_item))
#define DATA_START(h) (h + sizeof(struct heap_item) + (META_SIZE(h)))
#define HEAP_END(h)   (h + sizeof(struct heap_item) + (META_SIZE(h)) + (DATA_SIZE(h)))

struct heap {
    int survive_flag;
    size_t current_size, max_size;
    struct heap_item *list;
};

struct heap_item {
    char gcflag;
    int meta_size,  data_size;
    struct register_list *next;
};

void *allocate(size_t meta_size, size_t data_size);

void gc(struct heap *heap);

void free_heap(struct heap *heap);
#endif
