#include <stdio.h>
#include <stdlib.h>
#include "position.h"
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
    codegen->ins_num = codegen->cst_pool_num = 0;

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
    self->cst_pool_num++;

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
    self->ins_num++;

    return 0;
}

/* generate rawcode */
void *RawcodeGen_enerate(RawcodeGen *self) {

    const size_t cstpool_size = __CST_COUNT_SIZE + self->cst_pool_num * __CST_OFFSET_SIZE + self->cstpool_size;
    const size_t inslist_size = __INS_LENGTH_SIZE + self->ins_list_size;

    void *rawcode = malloc(__RAWCODE_OFFSET_SIZE + cstpool_size + inslist_size);
    
    void *cstpool = (void *)((char *)rawcode + __RAWCODE_OFFSET_SIZE);
    void *inslist = (void *)((char *)rawcode + __RAWCODE_OFFSET+ cstpool_size) 

    __InitRawcodeOffset(self, rawcode);
    
    __InitConstantPool(self, cstpool);

    __InitInstructionList(self, inslist);

    return rawcode;
}

void __InitRawcodeHead(RawcodeGen *self, void *rawcode) {

    __RAWCODE_OFFSET(rawcode)->cstpool = __RAWCODE_OFFSET_SIZE;
    __RAWCODE_OFFSET(rawcode)->inslist = __RAWCODE_OFFSET_SIZE + self->inslist_size;
}

void __InitConstantPool(RawcodeGen *self, void *cstpool) {

    int i;
    char *cursor;
    struct cstqueue *current;

    unsigned long int *cst_count = (unsigned long int *)cstpool, 
    unsigned long int  *cst_offsets = cstpool + __CST_COUNT_SIZE;
    char *cstpool_start_pos = (char *)cstpool + __CST_COUNT_SIZE + __CST_OFFSET_SIZE * self->cst_pool_num;

    *cst_count = self->cst_pool_num;

    cursor = cstpool_start_pos;
    current = self->cst_queue;
    for (i = 0; i < self->ins_num; i++) {
        memcpy(cursor, current->cst_queue, current->size); 
        cst_offsets[i] = cursor - cstpool_start_pos;

        cursor += size;
        current = current->next;
    }
}

void __InitInstructionList(RawcodeGen *self, void *inslist) {

    char *cursor;
    struct insqueue *current;

    unsigned long int *ins_list_size = (unsigned long int *)inslist;
    char *inslist_start_pos = (char *)inslist + __INS_LENGTH_SIZE;

    *ins_list_size = self->ins_list_size;    

    cursor = inslist_start_pos;
    current = self->ins_queue;
    while (cursor - inslist_start_pos < self->ins_list_size) {
        __WriteIns(cursor, current->ins);
        
        cursor += __GetInsSize(current->ins);
        current = current->next;
    }
}

void __WriteIns(char *pos, struct ins ins) {

    *pos = ins.id;
    pos++;
    
    switch (ins.id) {
    case ICONST: 
        *(int *)pos = ins.iconst;
        break;
    case FCONST:
        *(double *)pos = ins.fconst;
        break;
    case CCONST:
        *(char *)pos = ins.cconst;
        break;
    case JUMP:
        *(unsigned long int *)pos = ins.jump.addr;
        break;
    case JPZ:
        *(unsigned long int *)pos = ins.jump.addr;
        break;
    case JPNZ:
        *(unsigned long int *)pos = ins.jump.addr;
        break;
    case ALTSP:
        *(signed long int *)pos = ins.altsp.m;
        break;
    case CALL:
        *(unsigned int *)pos = ins.call.argnum; 
        pos += sizeof(unsigned int);
        *(unsigned long int *)pos = ins.call.addr;
        break;
    case TCALL:
        *(unsigned int *)pos = ins.call.new_argnum; 
        pos += sizeof(unsigned int);
        *(unsigned int *)pos = ins.call.old_argnum; 
        pos += sizeof(unsigned int);
        *(unsigned long int *)pos = ins.call.addr;
        break;
    case SYSCALL:
        *(unsigned int *)pos = ins.syscall.sysfuncid;
        break;
    case RAISE:
        *(unsigned int *)pos = ins.raise.exn;
        break;
    case PUSHEXN:
        *(unsigned int *)pos = ins.pushexn.exn;
        pos += sizeof(unsigned int);
        *(unsigned long int *)pos = ins.pushexn.addr;
        break;
    case IGETDATA: case FGETDATA: case CGETDATA:
        *(unsigned long int *)pos = ins.xgetdata.offset;
        break;
    case ISETDATA: case FSETDATA: case CSETDATA:
        *(unsigned long int *)pos = ins.xsetdata.offset;
        break;
    case GETREF:
        *(unsigned long int *)pos = ins.getref.offset;
        break;
    case SETREF:
        *(unsigned long int *)pos = ins.setref.offset;
        break;
    case NEW:
        *(unsigned long int *)pos = ins.new.refcnt;
        pos += sizeof(unsigned long int);
        *(unsigned long int *)pos = ins.new.datasz;
        break;
    case LDC:
        *(unsigned int)pos = ins.ldc.idx;
        break;
    }

    return;
}

void __Write
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
