#include <stdio.h>
#include <stdlib.h>
#include "rawcode.h"

#define INS_UNDEF(ins) (((ins) < ICONST) || ((ins) > LDC))

RawcodeGen *RawcodeGen_Init() {
    
    RawcodeGen *codegen = NULL;
    
    codegen = (RawcodeGen *)malloc(sizeof(RawcodeGen));
    if (codegen == NULL) {
        printf("Cannot allocate memory on heap.\n");
        abort();
    }
    
    codegen->ins_queue = NULL;
    codegen->cst_queue = NULL;
    codegen->ins_list_size = codegen->cst_pool_size = 0;

    return codegen;
}

void *RawcodeGen_AddConstant(RawcodeGen *self, size_t size) {
  
    struct cstqueue *new_cst_item;

    if (size == 0) {
        return NULL; 
    }

    /* initializing */
    new_cst_item = (struct cstqueue *)malloc(sizeof(struct cstqueue));
    if (new_cst_item == NULL) {
        printf("Cannot allocate memory on heap.\n");
        abort();
    }

    new_cst_item->ref = malloc(size);
    if (new_cst_item->ref == NULL) {
        printf("Cannot allocate memory on heap.\n");
        abort();
    }
    new_cst_item->size = size;

    if (self->cst_queue == NULL) {
        self->cst_queue = new_cst_item;
        self->cst_queue->next = self->cst_queue;
    }
    else {
        new_cst_item->next = self->cst_queue->next;
        self->cst_queue->next = new_cst_item;
    }
    
    self->cst_pool_size += size;
    return new_cst_item->ref;
}

int RawcodeGen_AddInstruction(RawcodeGen *self, struct ins *ins) {

    struct insqueue *new_ins_item;

    if (INS_UNDEF(ins->id)) {
        return -1;
    }
    
    new_ins_item = (struct insqueue *)malloc(sizeof(struct insqueue));
    if (new_ins_item == NULL) {
        printf("Cannot allocate memoery on heap.\n");
        abort();
    }

    new_ins_item->ins = *ins;

    if (self->ins_queue == NULL) {
        self->ins_queue = new_ins_item;
        self->ins_queue->next = self->ins_queue;
    }
    else {
        new_ins_item->next = self->ins_queue->next;
        self->ins_queue->next = new_ins_item;
    }

    self->ins_list_size += __GetInsSize(ins->id);
    return 0;
}

/* generate rawcode */
void *RawcodeGen_Generate(RawcodeGen *self) {
    
    return NULL;
}

void RawcodeGen_Delete(RawcodeGen *self) {
    __DeleteInsQueue(self->ins_queue);
    __DeleteCstQueue(self->cst_queue);
    
    free(self);
}

void __DeleteInsQueue(struct insqueue *q) {

    struct insqueue *temp = q;

    if (q == NULL) {
        return;
    }

    while (q != q->next) {
        q = q->next;

        free(temp);

        temp = q;
    }

    free(q);
}

void __DeleteCstQueue(struct cstqueue *q) {

    struct cstqueue *temp = q;

    if (q == NULL) {
        return;
    }

    while (q != q->next) {
        q = q->next;
        
        free(temp->ref);
        free(temp);

        temp = q;
    }
    
    free(q);
}

size_t __GetInsSize(int id) {

    size_t size;

    switch (id) {

    case ICONST:
        size = sizeof(unsigned char) + sizeof(int);
        break;

    case FCONST:
        size = sizeof(unsigned char) + sizeof(double);
        break;

    case CCONST:
        size = sizeof(unsigned char) << 1;
        break;

    case JUMP: case JPZ: case JPNZ:
        size = sizeof(unsigned char) + sizeof(unsigned long int);
        break;

    case CALL:
        size = sizeof(unsigned char) +  sizeof(unsigned int) + sizeof(unsigned long int);
        break;

    case TCALL:
        size = sizeof(unsigned char) + sizeof(unsigned int) << 1 + sizeof(unsigned long int);
        break;

    case SYSCALL:
        size = sizeof(unsigned char) + sizeof(unsigned int);
        break;

    case PUSHEXN:
        size = sizeof(unsigned char) + sizeof(unsigned int);
        break;

    case POPEXN:
        size = sizeof(unsigned char) + sizeof(unsigned int) + sizeof (unsigned long int);
        break;

    case LDC:
        size = sizeof(unsigned char) + sizeof(unsigned int);
        break;

    default:
       size = sizeof(char); 
       break;
    }

    return size;
}
