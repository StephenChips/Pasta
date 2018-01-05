#ifndef INCLUDED_RAWCODE_H
#define INCLUDED_RAWCODE_H

#include "instr.h"

#define SET_CONSTANT_FLOAT(constant, fval)
#define SET_CONSTANT_INT(constant, ival)
#define SET_CONSTANT_CHAR(constant, cval)
#define SET_CONSTANT_STRING(constant, sval, length)

struct insqueue {
    struct ins ins;
    struct insqueue *next;
};

struct cstqueue {
    void *ref;
    size_t size;
    struct cstqueue *next;
};

typedef struct __RawcodeGen {
    struct insqueue *ins_queue;
    struct cstqueue *cst_queue;
    size_t ins_list_size, cst_pool_size;
} RawcodeGen;


RawcodeGen *RawcodeGen_Init();

/*
 * PARAMS: 
 * rawcode: 
 *   type: struct rawcode *
 *   desc: the rawcode object
 * size:
 *   type: size_t
 *   desc: the size of rawcode
 *
 * DESC:
 *   This function will allocate a new memory in the constant linked-list, with the size of *size*, then return it.
 *   You can set the value of memeory to specific value by macros.
 */

void *RawcodeGen_AddConstant(RawcodeGen *self, size_t size);

int RawcodeGen_AddInstruction(RawcodeGen *self, struct ins *ins);

/* generate rawcode */
void *RawcodeGen_Generate(RawcodeGen *self);

void RawcodeGen_Delete(RawcodeGen *self);


/* private functions */
size_t __GetInsSize(int id);

void __DeleteInsQueue(struct insqueue *q);

void __DeleteCstQueue(struct cstqueue *q);

#endif
