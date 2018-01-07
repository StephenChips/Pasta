
void *AllocateHeapItem(struct heap_item_info) {
    
    void new_heap_item = malloc(sizeof(void *) * refcount + data_size);
    if (new_heap_item == NULL) {
        printf("Cannot allocate memory on heap.\n");
        abort();
    }

    __HEAPITEM_GC_FLAG(new_heap_item) = !info.gcflag
    __HEAPITEM_REF_COUNT(new_heap_item) = info.refcnt
    __HEAPITEM_DATA_SIZE(new_heap_item) = info.dtsz;

    return new_heap_item;
}

void *Heap_Allocate(Heap *self, unsigned long int refcount, size_t data_size) {

    struct heap_list *new_heap_item;
    struct heap_item_info info;

    new_heap_item = (struct heap_list *)malloc(sizeof(struct heap_list));
    if (new_heap_item == NULL) {
        printf("Cannot allocate memory on heap\n");
        abort();
    }

    info.gcflag = !vm.heap.survive_flag;
    info.refcnt = refcount;
    info.dtsz = data_size;

    new_heap_item->data = AllocateHeapItem(refcount, info);

    new_heap_item->next = vm.heap.list;
    vm.heap.list = new_heap_item->next;

    return new_heap_item->data;
}

void Heap_Gc(Heap *self) {

}

