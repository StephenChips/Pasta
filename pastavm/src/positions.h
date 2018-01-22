#ifndef POSITIONS_INCLUDED_H
#define POSITIONS_INCLUDED_H

/* This head file includes marcos and functions for accessing fields of structured raw memory block, like rawcode, heapitemect, constant pool, array etc. Since these memory contains some field whose length/size is not fixed at compiling time, it cannot represented through a C structure.
*/

#include "rawcode.h"

/* MACROS FOR RAWCODE OBJECT */
#define __RAWCODE_OFFSET(_rawcode) ((struct rawcode_offsets *) _rawcode)
#define __RAWCODE_OFFSET_SIZE sizeof(struct rawcode_offsets)
#define __RAWCODE_CSTPOOL_ADDR(_rawcode) (void *)((char *)(_rawcode) + __RAWCODE_OFFSET(_rawcode)->cstpool)
#define __RAWCODE_INSLIST_ADDR(_rawcode) (void *)((char *)(_rawcode) + __RAWCODE_OFFSET(_rawcode)->inslist)

/* MACROS FOR CONSTANT POOL */
#define __CST_COUNT(_cstpool) *((unsigned long int *) (_cstpool))
#define __CST_COUNT_SIZE sizeof(unsigned long int)
#define __CST_OFFSET_SIZE sizeof(unsigned long int)
#define __CST_OFFSET_ARRAY(_cstpool) ((unsigned long int *)((char *)(_cstpool) + __CST_COUNT_SIZE))
#define __CSTPOOL(_cstpool) ((char *)(_cstpool) + __CST_COUNT_SIZE + __CST_OFFSET_SIZE * __CST_COUNT(_cstpool))
#define __ACCESS_CONST(_cstpool, _idx) ((void *)((char *)_cstpool + __CST_OFFSET_ARRAY(_cstpool)[(_idx)]))

/* MACROS FOR INSTRUCTION LIST */
#define __INS_LENGTH(_inslist) *((unsigned long int *) (_inslist))
#define __INS_LENGTH_SIZE sizeof(unsigned long int)
#define __INS_ARRAY(_inslist) ((unsigned char *)(_inslist) + __INS_LENGTH_SIZE)


#define __ITEM_FLAG_VAL 1
#define __ITEM_SIZE sizeof(item_t)
#define __ITEM_FLAG(_item) ((_item) << (__ITEM_SIZE - 1))
#define __ITEM_ISREF(_item) (__ITEM_FLAG(_item) == __ITEM_FLAG_VAL)
#define __ITEM_DISABLE_FLAG(_itemptr) (*(_itemptr) = *(_itemptr) & (0xFFFFFFFFFFFFFFFF - 1))
#define __ITEM_ENABLE_FLAG(_item) (*(_item) = *(_item) | 0x1)


#define __ITEM_SET_INT(_itemptr, _ival) \
do { \
    *(_itemptr) = (long)(_ival) << 1; \
    __ITEM_DISABLE_FLAG(_itemptr);\
} while (0)

#define __ITEM_SET_FLOAT(_itemptr, _fval) \
do { \
    *(double *)(_itemptr) = (_fval); \
    __ITEM_DISABLE_FLAG(_itemptr); \
} while (0)

#define __ITEM_SET_CHAR(_itemptr, _cval) \
do { \
    *(_itemptr) = (long)(_cval) << 1;\
    __ITEM_DISABLE_FLAG(_itemptr);\
} while (0)

#define __ITEM_SET_REF(_itemptr, _rval) \
do {\
    *(_itemptr) = (long)(_rval) << 1; \
    __ITEM_ENABLE_FLAG(_itemptr);\
} while (0)

#define __ITEM_GET_INT(_itemptr) (int)(*(_itemptr) >> 1)

#define __ITEM_GET_CHAR(_itemptr) (char)(*(_itemptr) >> 1)

#define __ITEM_GET_REF(_itemptr) (void *)(*(_itemptr) >> 1)

#define __ITEM_GET_FLOAT(_itemptr) *(double *)(_itemptr)

/* MACROS FOR HEAP ITEM*/
/* heap item is a block of memory that contains a series of variables 
 * and references. Usually it allocated on heap at runtime, but it can locate in 
 * constant pool as well. 
 *
 * We also call `allocate` function while generating rawcode through `RawcodeGen` Object.
 */
#define DEFAULT_SURVIVE_FLAG 1

#define __HEAPITEM_INFO_SIZE (sizeof(struct heap_item_info)) 
#define __HEAPITEM_GC_FLAG(h)   (__HEAPITEM_INFO(h)->gcflag)
#define __HEAPITEM_ITEM_COUNT(h) (__HEAPITEM_INFO(h)->item_count)
#define __HEAPITEM_INFO(h)  ((struct heap_item_info *)h)
#define __HEAPITEM_DATA(h) ((item_t *)((char *)h + __HEAPITEM_INFO_SIZE))
#define __HEAPITEM_GETITEM(_h, _idx) (__HEAPITEM_DATA(h)[(_idx)])

/* MACROS FOR STACK LOCATION */
#define __CURRENT_ITEM_COUNT(_vm) ((item_t)((_vm).registers.sp - (_vm).stack.stack))
#define __AVALIABLE_STKSZ(_vm) ((_vm).stack.capacity - ((_vm).registers.sp - (_vm).stack.stack))

#define __IS_STACK_ADDR(vm, ref)  ((_ref) >= (_vm).stack.stack && (_ref) <= (_vm).stack.stack + (_vm.stack.capacity))
#define __IS_STK_FREAM_EXISTS(_vm) ((_vm).registers.bp != NULL)
#define __IS_EXN_HDR_EXISTS(_vm) ((_vm).registers.hr != NULL)

#define __STKFRAME_RETADDR(_bp) __ITEM_GET_INT(_bp)
#define __STKFRAME_OLDBP(_bp) __ITEM_GET_INT((_bp) + 1)

#define __EXNHDR_EXNID(_hr) __ITEM_GET_INT(_hr)
#define __EXNHDR_JUMPOFFSET(_hr) __ITEM_GET_INT((_hr) + 1)
#define __EXNHDR_OUTERHDR(_hr) __ITEM_GET_INT((_hr) + 2)

#define __IS_IDX_OUT_OF_CSTPOOL(_cstpool, idx) ((_idx) > __CST_COUNT(_cstpool))

/* MACROS FOR INS */
#define __INS_ICONST_VALUE(_insptr) *(int *)((_insptr) + 1)
#define __INS_FCONST_VALUE(_insptr) *(double *)((_insptr) + 1)
#define __INS_CCONST_VALUE(_insptr) *(char *)((_insptr) + 1)

#endif
