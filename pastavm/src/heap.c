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
    vm.heap.current_size += item_count * __ITEM_SIZE + __HEAPITEM_INFO_SIZE;

    return new_heap_item->data;
}

void Heap_DeleteAll(Heap *self) {

    struct heap_list *temp = self->list;

    while (self->list != NULL) {
        self->list = self->list->next;
        free(temp);
        temp = self->list;
    }
}

void Heap_GC(Heap *self) {

    item_t *cursor = vm.stack.stack;

    while (cursor < vm.registers.sp) {
        if (__ITEM_ISREF(cursor)) {
            __Heap_MarkHeapItem(__ITEM_GET_REF(cursor));
        }
        cursor++;
    }

    __Heap_Sweep();
}


void __Heap_MarkHeapItem(void *heap_item) {

    int i;
    item_t item;

    if (__HEAPITEM_GC_FLAG(heap_item) == vm.heap.survive_flag) {
        return;
    }

    __HEAPITEM_GC_FLAG(heap_item) = vm.heap.survive_flag;

    for (i = 0; i < __HEAPITEM_ITEM_COUNT(heap_item); i++) {
        item = __HEAPITEM_GETITEM(heap_item, i);
        if (__ITEM_ISREF(&item)) {
            __Heap_MarkHeapItem(__ITEM_GET_REF(&item));
        }
    } 
}

void __Heap_Sweep() {

    struct heap_list *prev, *current;

    if (vm.heap.list == NULL) {
        return;
    }

    prev = vm.heap.list;
    current = prev->next;

    while (current != NULL) {
        if (__HEAPITEM_GC_FLAG(current->data) == vm.heap.survive_flag) {
            prev = prev->next;
            current = prev->next;
        }
        else {
            prev->next = current->next;
            free(current->data);
            free(current);

            if (prev->next != NULL) {
                prev = prev->next;
                current = prev->next;
            } else {
                current = NULL;
            }
        }
    }

    current = vm.heap.list;
    if (__HEAPITEM_GC_FLAG(current->data) != vm.heap.survive_flag) {
        vm.heap.list = current->next;
        free(current->data);
        free(current);
    }

    vm.heap.survive_flag = !vm.heap.survive_flag;
}
