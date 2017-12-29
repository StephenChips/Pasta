void *allocate(size_t meta_size, size_t data_size) {
    struct heap_item *new_heap_item;

    new_heap_item = (struct heap_item *)malloc(sizeof(struct heap_item + meta_size + data_size));

    new_heap_item.gcflag = !heap.survive_flag;
    new_heap_item.meta_size = meta_size;
    new_heap_item.data_size = data_size;
    new_heap_item.next = vm.heap.list;
    vm.heap.list = new_heap_item.next;

    return (void *)new_heap_item + sizeof(new_heap_item);
}

void gc(struct heap *heap) {

}

void free_heap(){

    struct heap_item *temp = vm.heap.list;

    while (heap.list != NULL) {
        heap.list = heap.list->next;
        free(temp);
        temp = heap.list;
    }

}
