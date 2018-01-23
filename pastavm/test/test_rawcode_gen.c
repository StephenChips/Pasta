#include <check.h>
#include <stdio.h>
#include <stdlib.h>

#include "instr.h"
#include "rawcode.h"
#include "positions.h"
#include "testhelper.h"

#define CHECK_ADD_CONSTANT_TO_RAWCODE_GENERATOR($size) \
do { \
    void *mem = NULL; \
    struct cstqueue *constant; \
    size_t old_cstpool_size; \
    const size_t allocate_size = $size; \
    RawcodeGen *codegen = RawcodeGen_Init(); \
\
    old_cstpool_size = codegen->cst_pool_size; \
\
    mem = __RawcodeGen_AddConst(codegen, allocate_size); \
\
    constant = codegen->cst_queue; \
\
    ck_assert_ptr_nonnull(constant->ref); \
    ck_assert_ptr_nonnull(mem); \
    ck_assert_int_eq(constant->size, allocate_size); \
\
    /* cstqueue is a cycle list */ \
    ck_assert_ptr_nonnull(constant->next); \
    ck_assert_int_eq(codegen->cst_pool_size - old_cstpool_size, allocate_size); \
    RawcodeGen_Delete(codegen); \
} while (0)

#define CHECK_ADD_CONSTANT_TO_RAWCODE_GENERATOR_WRONG_SIZE($size) \
do { \
    void *mem = NULL; \
    struct cstqueue *constant; \
    RawcodeGen *codegen = RawcodeGen_Init(); \
    size_t old_cstpool_size = codegen->cst_pool_size, allocate_size = ($size); \
\
    mem = __RawcodeGen_AddConst(codegen, allocate_size); \
\
    ck_assert_ptr_null(mem); \
    ck_assert_int_eq(codegen->cst_pool_size - old_cstpool_size, 0); \
\
    RawcodeGen_Delete(codegen); \
} while (0)

/* 
 * TYPE
 *   $codegen : RawcodeGen *
 *   $insptr  : struct ins *
 */
#define CHECK_ADD_INS_TO_RAWCODE_GENERATOR_BASE(_codegen, _ins) \
do { \
    size_t actual_inslen;\
    int ret = 0, old_inslist_size; \
    struct insqueue *ins_queue_item; \
\
    old_inslist_size = (_codegen)->ins_list_size;\
\
    ret = RawcodeGen_AddInstruction((_codegen), (_ins));\
\
    ins_queue_item = (_codegen)->ins_queue;\
    actual_inslen = (_codegen)->ins_list_size - old_inslist_size;\
\
    ck_assert_int_eq(ret, 0); /* no error */ \
    ck_assert_int_eq(_ins.id, ins_queue_item->ins.id); \
\
    ck_assert_int_eq(__GetInsSize(_ins), actual_inslen); \
 \
} while (0) 

#define CHECK_ADD_ICONST_TO_RAWCODE_GENERATOR(_intval)  \
do { \
    RawcodeGen *codegen = RawcodeGen_Init(); \
    struct ins ins;\
    struct insqueue *ins_queue_item;\
\
    ins.id = ICONST;\
    ins.args.iconst.val = (_intval);\
\
    CHECK_ADD_INS_TO_RAWCODE_GENERATOR_BASE(codegen, ins);\
    \
    ins_queue_item = codegen->ins_queue;\
    ck_assert_int_eq(ins_queue_item->ins.args.iconst.val, ins.args.iconst.val);\
\
    RawcodeGen_Delete(codegen); \
} while (0)

#define CHECK_ADD_FCONST_TO_RAWCODE_GENERATOR(_floatval)  \
do { \
    RawcodeGen *codegen = RawcodeGen_Init(); \
    struct ins ins;\
    struct insqueue *ins_queue_item;\
\
    ins.id = FCONST;\
    ins.args.fconst.val = (_floatval);\
\
    CHECK_ADD_INS_TO_RAWCODE_GENERATOR_BASE(codegen, ins);\
    \
    ins_queue_item = codegen->ins_queue;\
    ck_assert_float_eq(ins_queue_item->ins.args.fconst.val, ins.args.fconst.val);\
\
    RawcodeGen_Delete(codegen); \
} while (0)

#define CHECK_ADD_CCONST_TO_RAWCODE_GENERATOR(_charval)  \
do { \
    RawcodeGen *codegen = RawcodeGen_Init(); \
    struct ins ins;\
    struct insqueue *ins_queue_item;\
\
    ins.id = CCONST;\
    ins.args.cconst.val = (_charval);\
\
    CHECK_ADD_INS_TO_RAWCODE_GENERATOR_BASE(codegen, ins);\
    \
    ins_queue_item = codegen->ins_queue;\
    ck_assert_int_eq(ins_queue_item->ins.args.cconst.val, ins.args.cconst.val);\
\
    RawcodeGen_Delete(codegen); \
} while (0)

#define CHECK_ADD_JUMP_TO_RAWCODE_GENERATOR(_addr)  \
do { \
    RawcodeGen *codegen = RawcodeGen_Init(); \
    struct ins ins;\
    struct insqueue *ins_queue_item;\
\
    ins.id = JUMP; \
    ins.args.jxxx.addr = (_addr); \
\
    CHECK_ADD_INS_TO_RAWCODE_GENERATOR_BASE(codegen, ins); \
\
    ins_queue_item = codegen->ins_queue; \
    ck_assert_int_eq(ins_queue_item->ins.args.jxxx.addr, ins.args.jxxx.addr); \
    RawcodeGen_Delete(codegen); \
} while (0) 

#define CHECK_ADD_JPZ_TO_RAWCODE_GENERATOR(_addr)  \
do { \
    RawcodeGen *codegen = RawcodeGen_Init(); \
    struct ins ins;\
    struct insqueue *ins_queue_item;\
\
    ins.id = JPZ; \
    ins.args.jxxx.addr = (_addr); \
\
    CHECK_ADD_INS_TO_RAWCODE_GENERATOR_BASE(codegen, ins); \
\
    ins_queue_item = codegen->ins_queue; \
    ck_assert_int_eq(ins_queue_item->ins.args.jxxx.addr, ins.args.jxxx.addr); \
    RawcodeGen_Delete(codegen); \
} while (0) 

#define CHECK_ADD_JPNZ_TO_RAWCODE_GENERATOR(_addr)  \
do { \
    RawcodeGen *codegen = RawcodeGen_Init(); \
    struct ins ins;\
    struct insqueue *ins_queue_item;\
\
    ins.id = JPNZ; \
    ins.args.jxxx.addr = (_addr); \
\
    CHECK_ADD_INS_TO_RAWCODE_GENERATOR_BASE(codegen, ins); \
\
    ins_queue_item = codegen->ins_queue; \
    ck_assert_int_eq(ins_queue_item->ins.args.jxxx.addr, ins.args.jxxx.addr); \
    RawcodeGen_Delete(codegen); \
} while (0) 

#define CHECK_ADD_ALTSP_TO_RAWCODE_GENERATOR(_m)  \
do { \
    RawcodeGen *codegen = RawcodeGen_Init(); \
    struct ins ins;\
    struct insqueue *ins_queue_item;\
\
    ins.id = ALTSP; \
    ins.args.altsp.m = (_m); \
\
    CHECK_ADD_INS_TO_RAWCODE_GENERATOR_BASE(codegen, ins); \
\
    ins_queue_item = codegen->ins_queue; \
    ck_assert_int_eq(ins_queue_item->ins.args.altsp.m, ins.args.altsp.m); \
    RawcodeGen_Delete(codegen); \
} while (0) 

#define CHECK_ADD_CALL_TO_RAWCODE_GENERATOR(_argnum, _addr)  \
do { \
    RawcodeGen *codegen = RawcodeGen_Init(); \
    struct ins ins;\
    struct insqueue *ins_queue_item;\
\
    ins.id = CALL; \
    ins.args.call.argnum = (_argnum); \
    ins.args.call.addr = (_addr); \
\
    CHECK_ADD_INS_TO_RAWCODE_GENERATOR_BASE(codegen, ins); \
\
    ins_queue_item = codegen->ins_queue; \
    ck_assert_int_eq(ins_queue_item->ins.args.call.argnum, ins.args.call.argnum); \
    ck_assert_int_eq(ins_queue_item->ins.args.call.addr, ins.args.call.addr); \
    RawcodeGen_Delete(codegen); \
} while (0) 

#define CHECK_ADD_TCALL_TO_RAWCODE_GENERATOR(_argnum, _addr)  \
do { \
    RawcodeGen *codegen = RawcodeGen_Init(); \
    struct ins ins;\
    struct insqueue *ins_queue_item;\
\
    ins.id = TCALL; \
    ins.args.call.argnum = (_argnum); \
    ins.args.call.addr = (_addr); \
\
    CHECK_ADD_INS_TO_RAWCODE_GENERATOR_BASE(codegen, ins); \
\
    ins_queue_item = codegen->ins_queue; \
    ck_assert_int_eq(ins_queue_item->ins.args.tcall.argnum, ins.args.tcall.argnum); \
    ck_assert_int_eq(ins_queue_item->ins.args.tcall.addr, ins.args.tcall.addr); \
    RawcodeGen_Delete(codegen); \
} while (0) 

#define CHECK_ADD_PUSHEXN_TO_RAWCODE_GENERATOR(_exn)  \
do { \
    RawcodeGen *codegen = RawcodeGen_Init(); \
    struct ins ins;\
    struct insqueue *ins_queue_item;\
\
    ins.id = PUSHEXN; \
    ins.args.pushexn.exn = (_exn); \
\
    CHECK_ADD_INS_TO_RAWCODE_GENERATOR_BASE(codegen, ins); \
\
    ins_queue_item = codegen->ins_queue; \
    ck_assert_int_eq(ins_queue_item->ins.args.pushexn.exn, ins.args.pushexn.exn); \
    RawcodeGen_Delete(codegen); \
} while (0) 

#define CHECK_ADD_POPEXN_TO_RAWCODE_GENERATOR(_exn, _addr)  \
do { \
    RawcodeGen *codegen = RawcodeGen_Init(); \
    struct ins ins;\
    struct insqueue *ins_queue_item;\
\
    ins.id = POPEXN;\
    ins.args.popexn.exn = (_exn); \
    ins.args.popexn.addr = (_addr); \
\
    CHECK_ADD_INS_TO_RAWCODE_GENERATOR_BASE(codegen, ins); \
\
    ins_queue_item = codegen->ins_queue; \
    ck_assert_int_eq(ins_queue_item->ins.args.popexn.exn, ins.args.popexn.exn); \
    ck_assert_int_eq(ins_queue_item->ins.args.popexn.addr, ins.args.popexn.addr); \
\
    RawcodeGen_Delete(codegen); \
} while (0) 

#define CHECK_ADD_ILDC_TO_RAWCODE_GENERATOR(_idx)  \
do { \
    RawcodeGen *codegen = RawcodeGen_Init(); \
    struct ins ins;\
    struct insqueue *ins_queue_item;\
\
    ins.id = ILDC; \
    ins.args.ldc.idx = (_idx); \
\
    CHECK_ADD_INS_TO_RAWCODE_GENERATOR_BASE(codegen, ins); \
\
    ins_queue_item = codegen->ins_queue; \
    ck_assert_int_eq(ins_queue_item->ins.args.xldc.idx, ins.args.xldc.idx); \
\
    RawcodeGen_Delete(codegen); \
} while (0) 

#define CHECK_ADD_UNDEFINS_TO_RAWCODE_GENERATOR_BASE(_insid) \
do { \
    struct ins ins; \
    int ret = 0, old_inslist_size; \
    Rawcode *codegen = Rawcode_Init(); \
\
    ins.id = (_insid); \
\
    old_inslist_size = codegen->ins_list_size;\
\
    ret = RawcodeGen_AddInstruction(codegen, ins);\
\
    ck_assert_int_eq(ret, -1); /* error */ \
    ck_assert_int_eq(old_inslist_size, codegen->ins_list_size) \
 \
    Rawcode_Delete(codegen); \
} while (0) 

#define ASSERT_INS_EQ(_expect, _actual, _size)  ck_assert_mem_eq(&(_expect), &(_actual), (_size));

/*
 * inslist: char *
 * expectlen: int
 * expectlist: struct ins []
 */

START_TEST(test_rawcode_generate_2) /* one instruction and no constant */
{
    RawcodeGen *codegen = RawcodeGen_Init();
    
    struct ins ins, instr;
    int insnum = 1;
    int inslist_len = 5;

    int cstpool_size = __CST_COUNT_SIZE;

    ins.id = ICONST;
    ins.args.iconst.val = 10;

    RawcodeGen_AddInstruction(codegen, ins);
    Rawcode *rawcode = RawcodeGen_Generate(codegen);

    ck_assert_int_eq(__RAWCODE_OFFSET(rawcode->rawcode)->cstpool, __RAWCODE_OFFSET_SIZE);
    ck_assert_int_eq(__RAWCODE_OFFSET(rawcode->rawcode)->inslist, __RAWCODE_OFFSET_SIZE + cstpool_size);

    void *cstpool = (void *)((char *)rawcode->rawcode + __RAWCODE_OFFSET_SIZE);
    void *inslist = (void *)((char *)rawcode->rawcode + __RAWCODE_OFFSET_SIZE + cstpool_size); 

    ck_assert_int_eq(__INS_LENGTH(inslist), inslist_len);
   
    size_t size = __GetInsSize(ins); 

    TestHelper_ReadIns(__INS_ARRAY(inslist), &instr);

    ck_assert_int_eq(is_instr_eq(ins, instr), 1);

    RawcodeGen_Delete(codegen);
    Rawcode_Delete(rawcode);
}
END_TEST

START_TEST(test_rawcode_generate_3) /* three instructions and no constant */
{
    RawcodeGen *codegen = RawcodeGen_Init();
    
    struct ins ins[3];
    int insnum = 3;

    ins[0].id = ICONST;
    ins[0].args.iconst.val = 10;
    ins[1].id = ICONST;
    ins[1].args.iconst.val = 100;
    ins[2].id = IADD;

    int inslist_len = __GetInsSize(ins[0]) + __GetInsSize(ins[1]) + __GetInsSize(ins[2]);
    int cstpool_size = __CST_COUNT_SIZE;


    RawcodeGen_AddInstruction(codegen, ins[0]);
    RawcodeGen_AddInstruction(codegen, ins[1]);
    RawcodeGen_AddInstruction(codegen, ins[2]);

    Rawcode *rawcode = RawcodeGen_Generate(codegen);

    ck_assert_int_eq(__RAWCODE_OFFSET(rawcode->rawcode)->cstpool, __RAWCODE_OFFSET_SIZE);
    ck_assert_int_eq(__RAWCODE_OFFSET(rawcode->rawcode)->inslist, __RAWCODE_OFFSET_SIZE + cstpool_size);

    void *cstpool = (void *)((char *)rawcode->rawcode + __RAWCODE_OFFSET_SIZE);
    void *inslist = (void *)((char *)rawcode->rawcode + __RAWCODE_OFFSET_SIZE + cstpool_size);

    ck_assert_int_eq(__INS_LENGTH(inslist), inslist_len);
  
    int i;
    unsigned char *cursor = __INS_ARRAY(inslist);
    printf("%ld\n", cursor - (unsigned char *)rawcode->rawcode);
    for (i = 0; i < insnum; i++) {
        struct ins instr;
        size_t size = __GetInsSize(ins[i]); 
        TestHelper_ReadIns(cursor, &instr);
        ck_assert_int_eq(is_instr_eq(ins[i], instr), 1);
        cursor += size;
    } 

    RawcodeGen_Delete(codegen);
    Rawcode_Delete(rawcode);
}
END_TEST


/* TEST ADD CONSTANT TO RAWCODE GENERATOR
  Permise:
 *   1. The Function `RawcodeGen_Init`, `RawcodeGen_Delete` should work correctly.
 */
START_TEST(test_rawcode_gen_init) 
{
    RawcodeGen *codegen = RawcodeGen_Init();    

    ck_assert_ptr_null(codegen->ins_queue);
    ck_assert_ptr_null(codegen->cst_queue);
    ck_assert_int_eq(codegen->ins_list_size, 0);
    ck_assert_int_eq(codegen->cst_pool_size, 0);

    RawcodeGen_Delete(codegen);
}
END_TEST

START_TEST(Test_RawcodeGen_AddConstant_1)
{
    CHECK_ADD_CONSTANT_TO_RAWCODE_GENERATOR(sizeof(int));
}
END_TEST

START_TEST(Test_RawcodeGen_AddConstant_2)
{
    CHECK_ADD_CONSTANT_TO_RAWCODE_GENERATOR(sizeof(double));
}
END_TEST

START_TEST(Test_RawcodeGen_AddConstant_3)
{
    CHECK_ADD_CONSTANT_TO_RAWCODE_GENERATOR(sizeof(char));
}
END_TEST

START_TEST(Test_RawcodeGen_AddConstant_4)
{
    CHECK_ADD_CONSTANT_TO_RAWCODE_GENERATOR(sizeof(void *));
}
END_TEST

START_TEST(Test_RawcodeGen_AddConstant_5)
{
    CHECK_ADD_CONSTANT_TO_RAWCODE_GENERATOR(1024L);
}
END_TEST

/* Error case */
START_TEST(Test_RawcodeGen_AddConstant_6)
{
    CHECK_ADD_CONSTANT_TO_RAWCODE_GENERATOR_WRONG_SIZE(0);
}
END_TEST

START_TEST(Test_RawcodeGen_AddIConst_1)
{
    CHECK_ADD_ICONST_TO_RAWCODE_GENERATOR(10);
}
END_TEST

START_TEST(Test_RawcodeGen_AddIConst_2)
{
    CHECK_ADD_ICONST_TO_RAWCODE_GENERATOR(0);
}
END_TEST

START_TEST(Test_RawcodeGen_AddIConst_3)
{
    CHECK_ADD_ICONST_TO_RAWCODE_GENERATOR(-10);
}
END_TEST

START_TEST(Test_RawcodeGen_AddIConst_4)
{
    CHECK_ADD_ICONST_TO_RAWCODE_GENERATOR(1 << 15);
}
END_TEST

START_TEST(Test_RawcodeGen_AddIConst_5)
{
    CHECK_ADD_ICONST_TO_RAWCODE_GENERATOR(- 1 << 15);
}
END_TEST


START_TEST(Test_RawcodeGen_AddFConst_1)
{
    CHECK_ADD_FCONST_TO_RAWCODE_GENERATOR(1.1234);
}
END_TEST

START_TEST(Test_RawcodeGen_AddFConst_2)
{
    CHECK_ADD_FCONST_TO_RAWCODE_GENERATOR(- 1.1234);
}
END_TEST

START_TEST(Test_RawcodeGen_AddFConst_3)
{
    CHECK_ADD_FCONST_TO_RAWCODE_GENERATOR(0.0);
}
END_TEST

START_TEST(Test_RawcodeGen_AddFConst_4)
{
    CHECK_ADD_FCONST_TO_RAWCODE_GENERATOR(10333.4331);
}
END_TEST

START_TEST(Test_RawcodeGen_AddFConst_5)
{
    CHECK_ADD_FCONST_TO_RAWCODE_GENERATOR(- 10333.4331);
}
END_TEST

START_TEST(Test_RawcodeGen_AddFConst_6)
{
    CHECK_ADD_FCONST_TO_RAWCODE_GENERATOR(10141234123352343.4331);
}
END_TEST


START_TEST(Test_RawcodeGen_AddFConst_7)
{
    CHECK_ADD_FCONST_TO_RAWCODE_GENERATOR(- 10141234123352343.4331);
}
END_TEST

START_TEST(Test_RawcodeGen_AddFConst_8)
{
    CHECK_ADD_FCONST_TO_RAWCODE_GENERATOR(- 1E30l);
}
END_TEST

START_TEST(Test_RawcodeGen_AddFConst_9)
{
    CHECK_ADD_FCONST_TO_RAWCODE_GENERATOR(1E30l);
}
END_TEST

START_TEST(Test_RawcodeGen_AddCConst_1)
{
    CHECK_ADD_CCONST_TO_RAWCODE_GENERATOR(-(char)65);
}
END_TEST

START_TEST(Test_RawcodeGen_AddCConst_2)
{
    CHECK_ADD_CCONST_TO_RAWCODE_GENERATOR((char)65);
}
END_TEST

START_TEST(Test_RawcodeGen_AddCConst_3)
{
    CHECK_ADD_CCONST_TO_RAWCODE_GENERATOR((char)0);
}
END_TEST

START_TEST(Test_RawcodeGen_AddCConst_4)
{
   CHECK_ADD_CCONST_TO_RAWCODE_GENERATOR((char)0xFF);
}
END_TEST

START_TEST(Test_RawcodeGen_AddCConst_5)
{
   CHECK_ADD_CCONST_TO_RAWCODE_GENERATOR((char)-0xFF);
}
END_TEST

START_TEST(Test_RawcodeGen_AddJump_1)
{
   CHECK_ADD_JUMP_TO_RAWCODE_GENERATOR(0xFFAADDCC);
}
END_TEST

START_TEST(Test_RawcodeGen_AddJump_2)
{
   CHECK_ADD_JUMP_TO_RAWCODE_GENERATOR(0x00000000);
}
END_TEST

START_TEST(Test_RawcodeGen_AddJpz_1)
{
   CHECK_ADD_JPZ_TO_RAWCODE_GENERATOR(0x00000000);
}
END_TEST

START_TEST(Test_RawcodeGen_AddJpz_2)
{
   CHECK_ADD_JPZ_TO_RAWCODE_GENERATOR(0x00000000);
}
END_TEST

START_TEST(Test_RawcodeGen_AddJpnz_1)
{
   CHECK_ADD_JPNZ_TO_RAWCODE_GENERATOR(0x00000000);
}
END_TEST

START_TEST(Test_RawcodeGen_AddJpnz_2)
{
   CHECK_ADD_JPNZ_TO_RAWCODE_GENERATOR(0x00000000);
}
END_TEST

START_TEST(Test_RawcodeGen_AddAltsp_1)
{
   CHECK_ADD_ALTSP_TO_RAWCODE_GENERATOR(1);
}
END_TEST

START_TEST(Test_RawcodeGen_AddAltsp_2)
{
   CHECK_ADD_ALTSP_TO_RAWCODE_GENERATOR(-1L);
}
END_TEST

START_TEST(Test_RawcodeGen_AddAltsp_3)
{
   CHECK_ADD_ALTSP_TO_RAWCODE_GENERATOR(345L);
}
END_TEST

START_TEST(Test_RawcodeGen_AddAltsp_4)
{
   CHECK_ADD_ALTSP_TO_RAWCODE_GENERATOR(-345L);
}
END_TEST

START_TEST(Test_RawcodeGen_AddAltsp_5)
{
   CHECK_ADD_ALTSP_TO_RAWCODE_GENERATOR(- 1 << 31);
}
END_TEST

START_TEST(Test_RawcodeGen_AddAltsp_6)
{
   CHECK_ADD_ALTSP_TO_RAWCODE_GENERATOR(- 1 << 31);
}
END_TEST

START_TEST(Test_RawcodeGen_AddCall_1)
{
   CHECK_ADD_CALL_TO_RAWCODE_GENERATOR(0, 0x0000A9310);
}
END_TEST

START_TEST(Test_RawcodeGen_AddCall_2)
{
   CHECK_ADD_CALL_TO_RAWCODE_GENERATOR(10, 0x0000A9310);
}
END_TEST

START_TEST(Test_RawcodeGen_AddCall_3)
{
   CHECK_ADD_CALL_TO_RAWCODE_GENERATOR(10, 0x00000000);
}
END_TEST

START_TEST(Test_RawcodeGen_AddCall_4)
{
   CHECK_ADD_CALL_TO_RAWCODE_GENERATOR(0, 0x00000000);
}
END_TEST

START_TEST(Test_RawcodeGen_AddTcall_1)
{
   CHECK_ADD_TCALL_TO_RAWCODE_GENERATOR(0, 0x00000000);
}
END_TEST

START_TEST(Test_RawcodeGen_AddTcall_2)
{
   CHECK_ADD_TCALL_TO_RAWCODE_GENERATOR(10000, 0x00000000);
}
END_TEST


START_TEST(Test_RawcodeGen_AddTcall_3)
{
   CHECK_ADD_TCALL_TO_RAWCODE_GENERATOR(0, 0xABCDEFAB);
}
END_TEST

START_TEST(Test_RawcodeGen_AddTcall_4)
{
   CHECK_ADD_TCALL_TO_RAWCODE_GENERATOR(10000, 0xABCDEFAB);
}
END_TEST

Suite *test_rawcode_generator() {
    Suite *s = suite_create("RawcodeGenerator");;

    TCase *test_add_constant = tcase_create("Constant"),
          *test_add_iconst = tcase_create("IConst"),
          *test_add_fconst = tcase_create("FConst"),
          *test_add_cconst = tcase_create("CConst"),
          *test_add_jxxx = tcase_create("Jxxx"),
          *test_add_altsp = tcase_create("Altsp"),
          *test_add_call = tcase_create("Call"),
          *test_add_tcall = tcase_create("TCall"),
          *test_generate = tcase_create("Generate");

    /* test add cosntants */
    tcase_add_test(test_add_constant, Test_RawcodeGen_AddConstant_1);
    tcase_add_test(test_add_constant, Test_RawcodeGen_AddConstant_2);
    tcase_add_test(test_add_constant, Test_RawcodeGen_AddConstant_3);
    tcase_add_test(test_add_constant, Test_RawcodeGen_AddConstant_4);
    tcase_add_test(test_add_constant, Test_RawcodeGen_AddConstant_5);
    tcase_add_test(test_add_constant, Test_RawcodeGen_AddConstant_6);

    /* test add iconst instruction */
    tcase_add_test(test_add_iconst, Test_RawcodeGen_AddIConst_1);
    tcase_add_test(test_add_iconst, Test_RawcodeGen_AddIConst_2);
    tcase_add_test(test_add_iconst, Test_RawcodeGen_AddIConst_3);
    tcase_add_test(test_add_iconst, Test_RawcodeGen_AddIConst_4);
    tcase_add_test(test_add_iconst, Test_RawcodeGen_AddIConst_5);

    /* test add fconst instruction */
    tcase_add_test(test_add_fconst, Test_RawcodeGen_AddFConst_1);
    tcase_add_test(test_add_fconst, Test_RawcodeGen_AddFConst_2);
    tcase_add_test(test_add_fconst, Test_RawcodeGen_AddFConst_3);
    tcase_add_test(test_add_fconst, Test_RawcodeGen_AddFConst_4);
    tcase_add_test(test_add_fconst, Test_RawcodeGen_AddFConst_5);
    tcase_add_test(test_add_fconst, Test_RawcodeGen_AddFConst_6);
    tcase_add_test(test_add_fconst, Test_RawcodeGen_AddFConst_7);
    tcase_add_test(test_add_fconst, Test_RawcodeGen_AddFConst_8);

    /* test add cconst instruction */
    tcase_add_test(test_add_cconst, Test_RawcodeGen_AddCConst_1);
    tcase_add_test(test_add_cconst, Test_RawcodeGen_AddCConst_2);
    tcase_add_test(test_add_cconst, Test_RawcodeGen_AddCConst_3);
    tcase_add_test(test_add_cconst, Test_RawcodeGen_AddCConst_4);
    tcase_add_test(test_add_cconst, Test_RawcodeGen_AddCConst_5);

    /* test add jump, jpz and jpnz instruction */
    tcase_add_test(test_add_jxxx, Test_RawcodeGen_AddJump_1);
    tcase_add_test(test_add_jxxx, Test_RawcodeGen_AddJump_2);

    tcase_add_test(test_add_jxxx, Test_RawcodeGen_AddJpz_1);
    tcase_add_test(test_add_jxxx, Test_RawcodeGen_AddJpz_2);

    tcase_add_test(test_add_jxxx, Test_RawcodeGen_AddJpnz_1);
    tcase_add_test(test_add_jxxx, Test_RawcodeGen_AddJpnz_2);

    /* test add altsp instruction */
    tcase_add_test(test_add_altsp, Test_RawcodeGen_AddAltsp_1);
    tcase_add_test(test_add_altsp, Test_RawcodeGen_AddAltsp_2);
    tcase_add_test(test_add_altsp, Test_RawcodeGen_AddAltsp_3);
    tcase_add_test(test_add_altsp, Test_RawcodeGen_AddAltsp_4);
    tcase_add_test(test_add_altsp, Test_RawcodeGen_AddAltsp_5);
    tcase_add_test(test_add_altsp, Test_RawcodeGen_AddAltsp_6);

    /* test add call instruction */
    tcase_add_test(test_add_call, Test_RawcodeGen_AddAltsp_1);
    tcase_add_test(test_add_call, Test_RawcodeGen_AddAltsp_2);
    tcase_add_test(test_add_call, Test_RawcodeGen_AddAltsp_3);
    tcase_add_test(test_add_call, Test_RawcodeGen_AddAltsp_4);

    /* test add tcall instruction */
    tcase_add_test(test_add_tcall, Test_RawcodeGen_AddTcall_1);
    tcase_add_test(test_add_tcall, Test_RawcodeGen_AddTcall_2);
    tcase_add_test(test_add_tcall, Test_RawcodeGen_AddTcall_3);
    tcase_add_test(test_add_tcall, Test_RawcodeGen_AddTcall_4);

    tcase_add_test(test_generate, test_rawcode_generate_2);
    tcase_add_test(test_generate, test_rawcode_generate_3);

    suite_add_tcase(s, test_add_constant);
    suite_add_tcase(s, test_add_iconst);
    suite_add_tcase(s, test_add_fconst);
    suite_add_tcase(s, test_add_cconst);
    suite_add_tcase(s, test_add_jxxx);
    suite_add_tcase(s, test_add_altsp);
    suite_add_tcase(s, test_add_call);
    suite_add_tcase(s, test_generate);

    return s;
}

int main() {
    
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = test_rawcode_generator();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_VERBOSE);

    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
