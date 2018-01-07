#ifndef INCLUDED_RAWCODE_H
#define INCLUDED_RAWCODE_H

#include "positions.h"
#include "instr.h"
#include "vm.h"

/* Assume that the allocated memory is enough to store the value, otherwise, the behaviour is undefined. */

/* PARMS:
 * constant : void *
 *  fval : double
 */

struct insqueue {
    struct ins ins;
    struct insqueue *next;
};

struct cstqueue {
    void *ref;
    size_t size;
    struct cstqueue *next;
};

struct rawcode_offsets {
    unsigned long int cstpool, inslist;
};

typedef struct __RawcodeGen {
    struct insqueue *ins_queue;
    struct cstqueue *cst_queue;
    size_t ins_list_size, cst_pool_size;
    unsigned int ins_num, cst_pool_num;
} RawcodeGen;

struct __allocfun {
    void *(malloc)(size_t);
    void *(allocate_heap)(unsigned long int, size_t);
};

RawcodeGen *RawcodeGen_Init();

void RawcodeGen_Delete(RawcodeGen *self);

void RawcodeGen_AddFloatConst(RawcodeGen *self, double fval);

void RawcodeGen_AddIntConst(RawcodeGen *self, int ival);

void RawcodeGen_AddCharConst(RawcodeGen *self, char cval);

void RawcodeGen_AddStringConst(RawcodeGen *self, const char *sval, size_t slen);

int RawcodeGen_AddInstruction(RawcodeGen *self, struct ins ins);

/* generate rawcode */
void *RawcodeGen_Generate(RawcodeGen *self);



/* private functions */
size_t __GetInsSize(int id);

void *__RawcodeGen_AddConst(RawcodeGen *self, size_t size, struct kkkk);

void __DeleteInsQueue(struct insqueue *q);

void __DeleteCstQueue(struct cstqueue *q);

#endif
