#include <stdlib.h>
#include <stdio.h>
#include "positions.h"
#include "heap.h"

void *AllocateHeapItem(struct heap_item_info info) {
    
    void *new_heap_item = malloc(__HEAPITEM_INFO_SIZE + info.item_count * __ITEM_SIZE);
    if (new_heap_item == NULL) {
        printf("Cannot allocate memory on heap.\n");
        abort();
    }

    __HEAPITEM_GC_FLAG(new_heap_item) = !info.gcflag;
    __HEAPITEM_ITEM_COUNT(new_heap_item) = info.item_count;

    return new_heap_item;
}

void *Heap_Allocate(Heap *self, size_t item_count) {

    struct heap_list *new_heap_item;
    struct heap_item_info info;

    new_heap_item = (struct heap_list *)malloc(sizeof(struct heap_list));
    if (new_heap_item == NULL) {
        printf("Cannot allocate memory on heap\n");
        abort();
    }

    info.gcflag = !vm.heap.survive_flag;
    info.item_count = item_count;

    new_heap_item->data = AllocateHeapItem(info);

    new_heap_item->next = vm.heap.list;
    vm.heap.list = new_heap_item->next;

    return new_heap_item->data;
}

void Heap_Gc(Heap *self) {

}

