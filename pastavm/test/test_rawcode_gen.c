#include <check.h>
#include <stdlib.h>
#include "instr.h"
#include "rawcode.h"

#define DIFF(a, b) ((a) - (b))
#define CHECK_ADD_CONSTANT_TO_RAWCODE_GENERATOR($size) \
do { \
    void *mem = NULL; \
    struct cstqueue *constant; \
    size_t old_cstpool_size; \
    const size_t allocate_size = $size; \
\
    RawcodeGen *codegen = RawcodeGen_Init(); \
    old_cstpool_size = codegen->cst_pool_size; \
\
    mem = RawcodeGen_AddConstant(codegen, allocate_size); \
\
    constant = codegen->cst_queue; \
\
    ck_assert_ptr_nonnull(constant->ref); \
    ck_assert_int_eq(constant->size, allocate_size); \
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
    mem = RawcodeGen_AddConstant(codegen, allocate_size); \
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
#define CHECK_ADD_INS_TO_RAWCODE_GENERATOR_BASE(_codegen, _insptr, _expected_len_size) \
do { \
    size_t actual_inslen;\
    int ret = 0, old_inslist_size; \
    struct insqueue *ins_queue_item; \
\
    old_inslist_size = (_codegen)->ins_list_size;\
\
    ret = RawcodeGen_AddInstruction((_codegen), (_insptr));\
\
    ins_queue_item = (_codegen)->ins_queue;\
    actual_inslen = (_codegen)->ins_list_size - old_inslist_size;\
\
    ck_assert_int_eq(ret, 0); /* no error */ \
    ck_assert_int_eq((_insptr)->id, ins_queue_item->ins.id); \
\
    ck_assert_int_eq(_expected_inslen, actual_inslen) \
 \
} while (0) 

#define CHECK_ADD_ICONST_TO_RAWCODE_GENERATOR(_intval)  \
do { \
    RawcodeGen *codegen = RawcodeGen_Init(); \
    struct ins ins;\
    struct insqueue *ins_queue_item;\
\
    ins.id = ICONST;\
    ins.args.iconst = (_intval);\
\
    CHECK_ADD_INS_TO_RAWCODE_GENERATOR_BASE(codegen, &ins, ICONST_SIZE);\
    \
    ins_queue_item = codegen->ins_queue;\
    ck_assert_int_eq(ins_queue_item->ins.args.iconst, ins.args.iconst);\
\
    RawcodeGen_Delete(codegen); \
while (0)

#define CHECK_ADD_FCONST_TO_RAWCODE_GENERATOR(_floatval)  \
do { \
    RawcodeGen *codegen = RawcodeGen_Init(); \
    struct ins ins;\
    struct insqueue *ins_queue_item;\
\
    ins.id = FCONST;\
    ins.args.fconst = (_floatval);\
\
    CHECK_ADD_INS_TO_RAWCODE_GENERATOR_BASE(codegen, &ins, FCONST_SIZE);\
    \
    ins_queue_item = codegen->ins_queue;\
    ck_assert_float_eq(ins_queue_item->ins.args.fconst, ins.args.fconst);\
\
    RawcodeGen_Delete(codegen); \
while (0)

#define CHECK_ADD_CCONST_TO_RAWCODE_GENERATOR(_charval)  \
do { \
    RawcodeGen *codegen = RawcodeGen_Init(); \
    struct ins ins;\
    struct insqueue *ins_queue_item;\
\
    ins.id = CCONST;\
    ins.args.cconst = (_charval);\
\
    CHECK_ADD_INS_TO_RAWCODE_GENERATOR_BASE(codegen, &ins, CCONST_SIZE);\
    \
    ins_queue_item = codegen->ins_queue;\
    ck_assert_float_eq(ins_queue_item->ins.args.cconst, ins.args.cconst);\
\
    RawcodeGen_Delete(codegen); \
while (0)

#define CHECK_ADD_JUMP_TO_RAWCODE_GENERATOR(_addr)  \
do { \
    RawcodeGen *codegen = RawcodeGen_Init(); \
    struct ins ins;\
    struct insqueue *ins_queue_item;\
\
    ins.id = (_insid); \
    ins.args.jump.addr = (_addr); \
\
    CHECK_ADD_INS_TO_RAWCODE_GENERATOR_BASE(codegen, &ins, JUMP); \
\
    ins_queue_item = codegen->ins_queue; \
    ck_assert_int_eq(ins_queue_item->ins.args.jump.addr, ins.args.jump.addr); \
    RawcodeGen_Delete(codegen); \
} while (0) 

#define CHECK_ADD_JPZ_TO_RAWCODE_GENERATOR(_addr)  \
do { \
    RawcodeGen *codegen = RawcodeGen_Init(); \
    struct ins ins;\
    struct insqueue *ins_queue_item;\
\
    ins.id = (_insid); \
    ins.args.jpz.addr = (_addr); \
\
    CHECK_ADD_INS_TO_RAWCODE_GENERATOR_BASE(codegen, &ins, JPZ); \
\
    ins_queue_item = codegen->ins_queue; \
    ck_assert_int_eq(ins_queue_item->ins.args.jpz.addr, ins.args.jpz.addr); \
    RawcodeGen_Delete(codegen); \
} while (0) 

#define CHECK_ADD_JPNZ_TO_RAWCODE_GENERATOR(_addr)  \
do { \
    RawcodeGen *codegen = RawcodeGen_Init(); \
    struct ins ins;\
    struct insqueue *ins_queue_item;\
\
    ins.id = (_insid); \
    ins.args.jpz.addr = (_addr); \
\
    CHECK_ADD_INS_TO_RAWCODE_GENERATOR_BASE(codegen, &ins, JPNZ); \
\
    ins_queue_item = codegen->ins_queue; \
    ck_assert_int_eq(ins_queue_item->ins.args.jpnz.addr, ins.args.jpnz.addr); \
    RawcodeGen_Delete(codegen); \
} while (0) 

#define CHECK_ADD_JXXX_TO_RAWCODE_GENERATOR(_insid, _inssize, _addr)  \
do { \
    RawcodeGen *codegen = RawcodeGen_Init(); \
    struct ins ins;\
    struct insqueue *ins_queue_item;\
\
    ins.id = (_insid); \
    ins.args.jump.addr = (_addr); \
\
    CHECK_ADD_INS_TO_RAWCODE_GENERATOR_BASE(codegen, &ins, (_inssize)); \
\
    ins_queue_item = codegen->ins_queue; \
    ck_assert_int_eq(ins_queue_item->ins.args.jump.addr, ins.args.jump.addr); \
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
    CHECK_ADD_INS_TO_RAWCODE_GENERATOR_BASE(codegen, &ins, ALTSP_SIZE); \
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
    CHECK_ADD_INS_TO_RAWCODE_GENERATOR_BASE(codegen, &ins, CALL_SIZE); \
\
    ins_queue_item = codegen->ins_queue; \
    ck_assert_int_eq(ins_queue_item->ins.args.call.argnum, ins.args.call.argnum); \
    ck_assert_int_eq(ins_queue_item->ins.args.call.addr, ins.args.call.addr); \
    RawcodeGen_Delete(codegen); \
} while (0) 

#define CHECK_ADD_TCALL_TO_RAWCODE_GENERATOR(_old_argnum, _new_argnum, _addr)  \
do { \
    RawcodeGen *codegen = RawcodeGen_Init(); \
    struct ins ins;\
    struct insqueue *ins_queue_item;\
\
    ins.id = TCALL; \
    ins.args.tcall.old_argnum = (_old_argnum); \
    ins.args.tcall.new_argnum = (_new_argnum); \
    ins.args.tcall.addr = (_addr); \
\
    CHECK_ADD_INS_TO_RAWCODE_GENERATOR_BASE(codegen, &ins, TCALL_SIZE); \
\
    ins_queue_item = codegen->ins_queue; \
    ck_assert_int_eq(ins_queue_item->ins.args.tcall.old_argnum, ins.args.tcall.old_argnum); \
    ck_assert_int_eq(ins_queue_item->ins.args.tcall.new_argnum, ins.args.tcall.new_argnum); \
    ck_assert_int_eq(ins_queue_item->ins.args.tcall.addr, ins.args.tcall.addr); \
    RawcodeGen_Delete(codegen); \
} while (0) 

#define CHECK_ADD_SYSCALL_TO_RAWCODE_GENERATOR(_sysfuncid)  \
do { \
    RawcodeGen *codegen = RawcodeGen_Init(); \
    struct ins ins;\
    struct insqueue *ins_queue_item;\
\
    ins.id = TCALL; \
    ins.args.syscall.sysfuncid = (_sysfuncid); \
\
    CHECK_ADD_INS_TO_RAWCODE_GENERATOR_BASE(codegen, &ins, SYSCALL_SIZE); \
\
    ins_queue_item = codegen->ins_queue; \
    ck_assert_int_eq(ins_queue_item->ins.args.syscall.sysfuncid, ins.args.syscall.sysfuncid); \
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
    CHECK_ADD_INS_TO_RAWCODE_GENERATOR_BASE(codegen, &ins, PUSHEXN_SIZE); \
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
    CHECK_ADD_INS_TO_RAWCODE_GENERATOR_BASE(codegen, &ins, POPEXN_SIZE); \
\
    ins_queue_item = codegen->ins_queue; \
    ck_assert_int_eq(ins_queue_item->ins.args.popexn.exn, ins.args.popexn.exn); \
    ck_assert_int_eq(ins_queue_item->ins.args.popexn.addr, ins.args.popexn.addr); \
\
    RawcodeGen_Delete(codegen); \
} while (0) 

#define CHECK_ADD_LDC_TO_RAWCODE_GENERATOR(_idx)  \
do { \
    RawcodeGen *codegen = RawcodeGen_Init(); \
    struct ins ins;\
    struct insqueue *ins_queue_item;\
\
    ins.id = LDC; \
    ins.args.ldc.idx = (_idx) \
\
    CHECK_ADD_INS_TO_RAWCODE_GENERATOR_BASE(codegen, &ins, LDC_SIZE); \
\
    ins_queue_item = codegen->ins_queue; \
    ck_assert_int_eq(ins_queue_item->ins.args.ldc.idx, ins.args.ldc.idx); \
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
    ret = RawcodeGen_AddInstruction(codegen, &ins);\
\
    ck_assert_int_eq(ret, -1); /* error */ \
    ck_assert_int_eq(old_inslist_size, codegen->ins_list_size) \
 \
    Rawcode_Delete(codegen); \
} while (0) 

/* TEST ADD CONSTANT TO RAWCODE GENERATOR
 * Permise:
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

Suite *test_rawcode_generator() {
    Suite *s;
    TCase *tc_core;

    s = suite_create("RawcodeGenerator");
    
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, Test_RawcodeGen_AddConstant_1);
    tcase_add_test(tc_core, Test_RawcodeGen_AddConstant_2);
    tcase_add_test(tc_core, Test_RawcodeGen_AddConstant_3);
    tcase_add_test(tc_core, Test_RawcodeGen_AddConstant_4);
    tcase_add_test(tc_core, Test_RawcodeGen_AddConstant_5);
    tcase_add_test(tc_core, Test_RawcodeGen_AddConstant_6);
 
    suite_add_tcase(s, tc_core);

    return s;
}

int main() {
    
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = test_rawcode_generator();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);

    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
