#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "instr.h"
#include "positions.h"
#include "rawcode.h"
#include "heap.h"

#define INS_UNDEF(ins) (((ins.id) < ICONST) || ((ins.id) > RLDC))

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
        self->cst_queue = self->cst_queue->next;
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
/* need to be changed */
void RawcodeGen_AddStringConst(RawcodeGen *self, const char *sval) {

    struct cstqueue *new_cst_item;
    struct heap_item_info info;
    size_t slen = strlen(sval);
    int i;
    item_t *data;

    /* initializing */
    new_cst_item = (struct cstqueue *)malloc(sizeof(struct cstqueue));
    if (new_cst_item == NULL) {
        printf("Cannot allocate memory on heap.\n");
        abort();
    }

    info.gcflag = !GC_IGNORE_FLAG;
    info.item_count = slen + 1;

    new_cst_item->ref = AllocateHeapItem(info);

    if (new_cst_item->ref == NULL) {
        printf("Cannot allocate memory on heap.\n");
        abort();
    }

    data = __HEAPITEM_DATA(new_cst_item->ref);
    for (i = 0; i < slen; i++) {
        __ITEM_SET_CHAR(&(data[i]), sval[i]);
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
        self->cst_queue = self->cst_queue->next;
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
        self->ins_queue = self->ins_queue->next;
    }

    self->ins_list_size += __GetInsSize(ins);
    self->ins_num++;

    return 0;
}

/* generate rawcode */
Rawcode *RawcodeGen_Generate(RawcodeGen *self) {

    const size_t cstpool_size = __CST_COUNT_SIZE + self->cst_pool_num * __CST_OFFSET_SIZE + self->cst_pool_size;
    const size_t inslist_size = __INS_LENGTH_SIZE + self->ins_list_size;
    const size_t rawcode_size = __RAWCODE_OFFSET_SIZE + cstpool_size + inslist_size;

    void *rawcode = malloc(rawcode_size);
    
    void *cstpool = (void *)((char *)rawcode + __RAWCODE_OFFSET_SIZE);
    void *inslist = (void *)((char *)rawcode + __RAWCODE_OFFSET_SIZE + cstpool_size);

    __InitRawcodeHead(self, rawcode);
    
    __InitConstantPool(self, cstpool);

    __InitInstructionList(self, inslist);


    return Rawcode_Init(rawcode, rawcode_size);
}

void __InitRawcodeHead(RawcodeGen *self, void *rawcode) {

    unsigned long int cstpool_offset = __RAWCODE_OFFSET_SIZE;
    unsigned long int cstpool_size = __CST_COUNT_SIZE + __CST_OFFSET_SIZE * self->cst_pool_num + self->cst_pool_size;
    unsigned long int inslist_offset = __RAWCODE_OFFSET_SIZE + cstpool_size;

    __RAWCODE_OFFSET(rawcode)->cstpool = cstpool_offset;
    __RAWCODE_OFFSET(rawcode)->inslist = inslist_offset; 
    
}

void __InitConstantPool(RawcodeGen *self, void *cstpool) {

    __CST_COUNT(cstpool) = self->cst_pool_num;

    if (self->cst_queue == NULL) {
        return;
    }

    int i;
    char *cursor = (char *)cstpool + __CST_COUNT_SIZE + __CST_OFFSET_SIZE * self->cst_pool_num;
    struct cstqueue *current = self->cst_queue->next;
    for (i = 0; i < self->cst_pool_num; i++) {
        memcpy(cursor, current->ref, current->size); 
        __CST_OFFSET_ARRAY(cstpool)[i] = cursor - (char *)cstpool;

        cursor += current->size;
        current = current->next;
       /* i++; */ /* THIS STUBID BUD IS MEMORABLE! FUCK!!!!!*/
    }
}

void __InitInstructionList(RawcodeGen *self, void *inslist) {

    /* write length of instruction list, if there are no instruction */
    __INS_LENGTH(inslist) = self->ins_list_size;

    if (self->ins_queue == NULL) {
        return;
    }
    
    /* init instruction list */
    int i;
    char *cursor = (char *)inslist + __INS_LENGTH_SIZE;
    struct insqueue *current = self->ins_queue->next;
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
    case JUMP: case JPZ: case JPNZ:
        *(unsigned long int *)pos = ins.args.jxxx.addr;
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
    case IGETDATA: case FGETDATA: case CGETDATA: case RGETDATA:
        *(unsigned long int *)pos = ins.args.xgetdata.offset;
        break;
    case ISETDATA: case FSETDATA: case CSETDATA: case RSETDATA:
        *(unsigned long int *)pos = ins.args.xsetdata.offset;
        break;
    case NEW:
        *(unsigned long int *)pos = ins.args.new.refcnt;
        pos += sizeof(unsigned long int);
        *(unsigned long int *)pos = ins.args.new.datasz;
        break;
    case ILDC: case FLDC: case CLDC: case RLDC:
        *(unsigned int *)pos = ins.args.xldc.idx;
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

    case ILDC: case FLDC: case CLDC: case RLDC:
        size = sizeof(unsigned char) + sizeof(unsigned int);
        break;
    
    case ALTSP:
        size = sizeof(unsigned char) + sizeof(signed long int);
        break;
    default:
       size = sizeof(unsigned char); 
       break;
    }

    return size;
}

Rawcode *Rawcode_Init(void *rawcode, size_t size) {
    Rawcode *new_rawcode = (Rawcode *)malloc(sizeof(Rawcode));
    if (new_rawcode == NULL) {
        return NULL;
    }

    new_rawcode->rawcode = rawcode;
    new_rawcode->size = size;

    return new_rawcode;
}

void Rawcode_Delete(Rawcode *self) {
    free(self->rawcode);
    free(self);
}
