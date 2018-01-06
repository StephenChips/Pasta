#ifndef POSITIONS_INCLUDED_H
#define POSITIONS_INCLUDED_H

/* This head file includes marcos and functions for accessing fields of structured raw memory block, like rawcode, object, constant pool, array etc. Since these memory contains some field whose length/size is not fixed at compiling time, it cannot represented through a C structure.
*/



/* MACROS FOR RAWCODE OBJECT */
/*                           */
#define __RAWCODE_OFFSET(_rawcode) ((struct rawcode_offsets *) _rawcode)
#define __RAWCODE_OFFSET_SIZE sizeof(struct rawcode_offets)
#define __RAWCODE_CSTPOOL_ADDR(_rawcode) (void *)((char *)(_rawcode) + __RAWCODE_OFFSET(_rawcode)->cstpool)
#define __RAWCODE_INSLIST_ADDR(_rawcode) (void *)((char *)(_rawcode) + __RAWCODE_OFFSET(_rawcode)->inslist)

/* MACROS FOR CONSTANT POOL */
#define __CST_COUNT(_cstpool) *((unsigned long int *) (_cstpool))
#define __CST_COUNT_SIZE sizeof(unsigned long int)
#define __CST_OFFSET_SIZE sizeof(unsigned long int)
#define __CST_OFFSET_ARRAY(_cstpool) (unsigned long int *)((char *)(_cstpool) + __CST_COUNT_SIZE)

/* MACROS FOR INSTRUCTION LIST */
#define __INS_LENGTH(_inslist) *((unsigned long int *) (_inslist))
#define __INS_LENGTH_SIZE sizeof(unsigned long int)
#define __INS_ARRAY(_inslist) ((char *)(_inslist) + __INS_LENGTH_SIZE)



/* MACROS FOR OBJECT */
/* object is generally a block of memory that contains a series of variables and references.
 * which can be either on heap area or constant area.
 */
#define __OBJ_INFO_SIZE (sizeof(struct obj_info))

#define __OBJ_GC_FLAG(h)   (__OBJ_INFO(h)->gcflag)
#define __OBJ_REF_COUNT(h) (__OBJ_INFO(h)->refcnt)
#define __OBJ_DATA_SIZE(h) (__OBJ_INFO(h)->dtsz)

#define __OBJ_INFO(h)  ((struct obj_info *)h)
#define __OBJ_REF_ARRAY(h) (void **)((char *)h + __OBJ_INFO_SIZE)
#define __OBJ_DATA(h) (void *)((char *)h + __OBJ_INFO_SIZE + sizeof(void *)*__OBJ_REF_COUNT(h))


/* MACROS FOR STACK LOCATION */
#define __CMP_STKLEN_AND_OPRLEN(oprlen) /* TODO */
#define __CMP_RETLEN_AND_AVAILABLE_STKSZ(retlen) /* TODO */
#define __IS_REF_INSIDE_INSAREA(ref) /* TODO */
#define __IS_REF_INSIDE_HEAPAREA(ref) /* TODO */
#define __IS_STK_FREAM_EXISTS() /* TODO */
#define __IS_EXN_HDR_EXISTS /* TODO */
#define __IS_OFFSET_OUT_OF_META_AREA(offset) /* TODO */
#define __IS_OFFSET_OUT_OF_DATA_AREA(offset) /* TODO */
#define __IS_IDX_OUT_OF_CSTPOOL(idx) /* TODO */
#define __IS_IDX_OUT_OF_REF_AREA(idx) /* TODO */


struct obj_info {
    char gcflag;
    unsigned int refcnt;
    size_t dtsz;
};

struct rawcode_offsets {
    unsigned long int cstpool, inslist;
};

#endif
