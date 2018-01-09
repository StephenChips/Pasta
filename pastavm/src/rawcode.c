#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "instr.h"
#include "positions.h"
#include "rawcode.h"
#include "heap.h"

#define INS_UNDEF(ins) (((ins.id) < ICONST) || ((ins.id) > LDC))

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

void *__RawcodeGen_AddConst(RawcodeGen *self, size_t size) {
  
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

void RawcodeGen_AddFloatConst(RawcodeGen *self, double fval) {
    void *constant = __RawcodeGen_AddConst(self, sizeof(double));
    memcpy(constant, &fval, sizeof(double));
}

void RawcodeGen_AddIntConst(RawcodeGen *self, int ival) {
    void *constant = __RawcodeGen_AddConst(self, sizeof(int));
    memcpy(constant, &ival, sizeof(int));
}

void RawcodeGen_AddCharConst(RawcodeGen *self, char cval) {
    void *constant = __RawcodeGen_AddConst(self, sizeof(char));
    memcpy(constant, &cval, sizeof(char));
}

/* ASSUME STRING TERMIATED WITH '\0' */
void RawcodeGen_AddStringConst(RawcodeGen *self, const char *sval) {

    struct cstqueue *new_cst_item;
    struct heap_item_info info;
    size_t slen = strlen(sval);

    /* initializing */
    new_cst_item = (struct cstqueue *)malloc(sizeof(struct cstqueue));
    if (new_cst_item == NULL) {
        printf("Cannot allocate memory on heap.\n");
        abort();
    }

    info.gcflag = !GC_IGNORE_FLAG;
    info.refcnt = 0;
    info.dtsz = slen;

    new_cst_item->ref = AllocateHeapItem(info);

    if (new_cst_item->ref == NULL) {
        printf("Cannot allocate memory on heap.\n");
        abort();
    }
    strcpy((char *)__HEAPITEM_DATA(new_cst_item->ref), 
           sval);

    new_cst_item->size = slen + 1; /* add size of '\0' */

    if (self->cst_queue == NULL) {
        self->cst_queue = new_cst_item;
        self->cst_queue->next = self->cst_queue;
    }
    else {
        new_cst_item->next = self->cst_queue->next;
        self->cst_queue->next = new_cst_item;
    }
    
    self->cst_pool_size += slen + 1;
    self->cst_pool_num++;
}


int RawcodeGen_AddInstruction(RawcodeGen *self, struct ins ins) {

    struct insqueue *new_ins_item;

    if (INS_UNDEF(ins)) {
        return -1;
    }
    
    new_ins_item = (struct insqueue *)malloc(sizeof(struct insqueue));
    if (new_ins_item == NULL) {
        printf("Cannot allocate memoery on heap.\n");
        abort();
    }

    new_ins_item->ins = ins;

    if (self->ins_queue == NULL) {
        self->ins_queue = new_ins_item;
        self->ins_queue->next = self->ins_queue;
    }
    else {
        new_ins_item->next = self->ins_queue->next;
        self->ins_queue->next = new_ins_item;
    }

    self->ins_list_size += __GetInsSize(ins);
    self->ins_num++;

    return 0;
}

/* generate rawcode */
void *RawcodeGen_Generate(RawcodeGen *self) {

    const size_t cstpool_size = __CST_COUNT_SIZE + self->cst_pool_num * __CST_OFFSET_SIZE + self->cst_pool_size;
    const size_t inslist_size = __INS_LENGTH_SIZE + self->ins_list_size;

    void *rawcode = malloc(__RAWCODE_OFFSET_SIZE + cstpool_size + inslist_size);
    
    void *cstpool = (void *)((char *)rawcode + __RAWCODE_OFFSET_SIZE);
    void *inslist = (void *)((char *)rawcode + __RAWCODE_OFFSET_SIZE + cstpool_size);


    __InitRawcodeHead(self, rawcode);
    
    __InitConstantPool(self, cstpool);

    __InitInstructionList(self, inslist);

    return rawcode;
}

void __InitRawcodeHead(RawcodeGen *self, void *rawcode) {

    unsigned long int cstpool_offset = __RAWCODE_OFFSET_SIZE;
    unsigned long int cstpool_size = __CST_COUNT_SIZE + __CST_OFFSET_SIZE * self->cst_pool_num + self->cst_pool_size;
    unsigned long int inslist_offset = __RAWCODE_OFFSET_SIZE + cstpool_size;

    __RAWCODE_OFFSET(rawcode)->cstpool = cstpool_offset;
    __RAWCODE_OFFSET(rawcode)->inslist = inslist_offset; 
}

void __InitConstantPool(RawcodeGen *self, void *cstpool) {

    int i;
    char *cursor;
    struct cstqueue *current;

    unsigned long int *cst_count = (unsigned long int *)cstpool;
    unsigned long int  *cst_offsets = cstpool + __CST_COUNT_SIZE;
    char *cstpool_start_pos = (char *)cstpool + __CST_COUNT_SIZE + __CST_OFFSET_SIZE * self->cst_pool_num;

    *cst_count = self->cst_pool_num;

    cursor = cstpool_start_pos;
    current = self->cst_queue;
    printf("=======\n");
    for (i = 0; i < self->cst_pool_num; i++) {
        memcpy(cursor, current->ref, current->size); 
        cst_offsets[i] = cursor - (char *)cstpool;

        printf("size: %d\n", current->size);
        cursor += current->size;
        current = current->next;
       /* i++; */ /* THIS STUBID BUD IS MEMORABLE! FUCK!!!!!*/
    }
}

void __InitInstructionList(RawcodeGen *self, void *inslist) {

    int i;
    char *cursor;
    struct insqueue *current;

    unsigned long int *ins_list_size = (unsigned long int *)inslist;
    char *inslist_start_pos = (char *)inslist + __INS_LENGTH_SIZE;

    *ins_list_size = self->ins_list_size;    
    
    cursor = inslist_start_pos;
    current = self->ins_queue;
    for(i = 0; i < self->ins_num; i++) {
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
        *(int *)pos = ins.args.iconst.val;
        break;
    case FCONST:
        *(double *)pos = ins.args.fconst.val;
        break;
    case CCONST:
        *(char *)pos = ins.args.cconst.val;
        break;
    case JUMP:
        *(unsigned long int *)pos = ins.args.jump.addr;
        break;
    case JPZ:
        *(unsigned long int *)pos = ins.args.jump.addr;
        break;
    case JPNZ:
        *(unsigned long int *)pos = ins.args.jump.addr;
        break;
    case ALTSP:
        *(signed long int *)pos = ins.args.altsp.m;
        break;
    case CALL:
        *(unsigned int *)pos = ins.args.call.argnum; 
        pos += sizeof(unsigned int);
        *(unsigned long int *)pos = ins.args.call.addr;
        break;
    case TCALL:
        *(unsigned int *)pos = ins.args.tcall.new_argnum; 
        pos += sizeof(unsigned int);
        *(unsigned int *)pos = ins.args.tcall.old_argnum; 
        pos += sizeof(unsigned int);
        *(unsigned long int *)pos = ins.args.call.addr;
        break;
    case SYSCALL:
        *(unsigned int *)pos = ins.args.syscall.sysfuncid;
        break;
    case RAISE:
        *(unsigned int *)pos = ins.args.raise.exn;
        break;
    case PUSHEXN:
        *(unsigned int *)pos = ins.args.pushexn.exn;
        pos += sizeof(unsigned int);
        *(unsigned long int *)pos = ins.args.pushexn.addr;
        break;
    case IGETDATA: case FGETDATA: case CGETDATA:
        *(unsigned long int *)pos = ins.args.xgetdata.offset;
        break;
    case ISETDATA: case FSETDATA: case CSETDATA:
        *(unsigned long int *)pos = ins.args.xsetdata.offset;
        break;
    case GETREF:
        *(unsigned long int *)pos = ins.args.getref.offset;
        break;
    case SETREF:
        *(unsigned long int *)pos = ins.args.setref.offset;
        break;
    case NEW:
        *(unsigned long int *)pos = ins.args.new.refcnt;
        pos += sizeof(unsigned long int);
        *(unsigned long int *)pos = ins.args.new.datasz;
        break;
    case LDC:
        *(unsigned int *)pos = ins.args.ldc.idx;
        break;
    }

    return;
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
        temp = q->next;
        q->next = q->next->next;
        
        free(temp);
    }

    free(q);
}

void __DeleteCstQueue(struct cstqueue *q) {

    struct cstqueue *temp = q;

    if (q == NULL) {
        return;
    }

    while (q != q->next) {
        temp = q->next;
        q->next = q->next->next;
        
        free(temp->ref);
        free(temp);
    }
    
    free(q);
}

size_t __GetInsSize(struct ins ins) {

    size_t size;

    switch (ins.id) {

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
       size = sizeof(unsigned char); 
       break;
    }

    return size;
}
