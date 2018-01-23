#include <stdio.h>
#include <stdlib.h>
#include <check.h>

#include "rawcode.h"
#include "vm.h"
#include "errlog.h"
#include "cJSON.h"
#include "positions.h"

#define SAMPLE_RAWCODE_FILE_NAME "sample_rawcode.prc"

#define TEST_ICONST(_ival) \
do {\
    RawcodeGen *codegen = RawcodeGen_Init();\
    FILE *fp = fopen(SAMPLE_RAWCODE_FILE_NAME, "wb");\
    int ret;\
    struct ins ins;\
\
    ins.id = ICONST;\
    ins.args.iconst.val = (_ival); \
\
    RawcodeGen_AddInstruction(codegen, ins);\
    Rawcode *rawcode = RawcodeGen_Generate(codegen);\
\
    fwrite(rawcode->rawcode, rawcode->size, 1, fp);\
    fclose(fp);\
    \
    load(SAMPLE_RAWCODE_FILE_NAME);\
    execute();\
\
    ret = __ITEM_GET_INT(vm.registers.sp - 1);\
    ck_assert_int_eq(ret, (_ival));\
\
    Rawcode_Delete(rawcode);\
    halt();\
} while (0)

#define TEST_FCONST(_fval) \
do {\
    RawcodeGen *codegen = RawcodeGen_Init();\
    FILE *fp = fopen(SAMPLE_RAWCODE_FILE_NAME, "wb");\
    double ret;\
    struct ins ins;\
\
    ins.id = FCONST;\
    ins.args.fconst.val = (_fval); \
\
    RawcodeGen_AddInstruction(codegen, ins);\
    Rawcode *rawcode = RawcodeGen_Generate(codegen);\
\
    fwrite(rawcode->rawcode, rawcode->size, 1, fp);\
    fclose(fp);\
\
    load(SAMPLE_RAWCODE_FILE_NAME);\
    execute();\
\
    ret = __ITEM_GET_FLOAT(vm.registers.sp - 1);\
    ck_assert_double_eq(ret, (_fval));\
\
    Rawcode_Delete(rawcode);\
    halt();\
} while (0) 

#define TEST_CCONST(_cval) \
do {\
    RawcodeGen *codegen = RawcodeGen_Init();\
    FILE *fp = fopen(SAMPLE_RAWCODE_FILE_NAME, "wb");\
    char ret;\
    struct ins ins;\
\
    ins.id = CCONST;\
    ins.args.cconst.val = (_cval); \
\
    RawcodeGen_AddInstruction(codegen, ins);\
    Rawcode *rawcode = RawcodeGen_Generate(codegen);\
\
    fwrite(rawcode->rawcode, rawcode->size, 1, fp);\
    fclose(fp);\
\
    load(SAMPLE_RAWCODE_FILE_NAME);\
    execute();\
\
    ret = __ITEM_GET_CHAR(vm.registers.sp - 1);\
    ck_assert_int_eq(ret, (_cval));\
\
    Rawcode_Delete(rawcode);\
    halt();\
} while (0) 

#define TEST_IADD(_x, _y) \
do {\
    RawcodeGen *codegen = RawcodeGen_Init();\
    FILE *fp = fopen(SAMPLE_RAWCODE_FILE_NAME, "wb");\
    int ret;\
    struct ins ins[3];\
\
    ins[0].id = ins[1].id = ICONST; \
    ins[2].id = IADD;\
\
    ins[0].args.iconst.val = (_y);\
    ins[1].args.iconst.val = (_x);\
\
    int i;\
    for (i = 0; i < 3; i++) {\
        RawcodeGen_AddInstruction(codegen, ins[i]);\
    }\
    Rawcode *rawcode = RawcodeGen_Generate(codegen);\
    fwrite(rawcode->rawcode, rawcode->size, 1, fp);\
    fclose(fp);\
\
    load(SAMPLE_RAWCODE_FILE_NAME);\
    execute(SAMPLE_RAWCODE_FILE_NAME);\
\
    ret = __ITEM_GET_INT(vm.registers.sp - 1);\
    ck_assert_int_eq(ret, (_x) + (_y));\
\
    Rawcode_Delete(rawcode);\
    halt();\
} while (0)

#define TEST_FADD(_x, _y) \
do {\
    RawcodeGen *codegen = RawcodeGen_Init();\
    FILE *fp = fopen(SAMPLE_RAWCODE_FILE_NAME, "wb");\
    double ret;\
    struct ins ins[3];\
\
    ins[0].id = ins[1].id = FCONST; \
    ins[2].id = FADD;\
\
    ins[0].args.fconst.val = (_y);\
    ins[1].args.fconst.val = (_x);\
\
    int i;\
    for (i = 0; i < 3; i++) {\
        RawcodeGen_AddInstruction(codegen, ins[i]);\
    }\
    Rawcode *rawcode = RawcodeGen_Generate(codegen);\
    fwrite(rawcode->rawcode, rawcode->size, 1, fp);\
    fclose(fp);\
\
    load(SAMPLE_RAWCODE_FILE_NAME);\
    execute(SAMPLE_RAWCODE_FILE_NAME);\
\
    ret = __ITEM_GET_FLOAT(vm.registers.sp - 1);\
    ck_assert_float_eq(ret, (_x) + (_y));\
\
    Rawcode_Delete(rawcode);\
    halt();\
} while (0)

#define TEST_ISUB(_x, _y) \
do {\
    RawcodeGen *codegen = RawcodeGen_Init();\
    FILE *fp = fopen(SAMPLE_RAWCODE_FILE_NAME, "wb");\
    int ret;\
    struct ins ins[3];\
\
    ins[0].id = ins[1].id = ICONST; \
    ins[2].id = ISUB;\
\
    ins[0].args.iconst.val = (_y);\
    ins[1].args.iconst.val = (_x);\
\
    int i;\
    for (i = 0; i < 3; i++) {\
        RawcodeGen_AddInstruction(codegen, ins[i]);\
    }\
    Rawcode *rawcode = RawcodeGen_Generate(codegen);\
    fwrite(rawcode->rawcode, rawcode->size, 1, fp);\
    fclose(fp);\
\
    load(SAMPLE_RAWCODE_FILE_NAME);\
    execute(SAMPLE_RAWCODE_FILE_NAME);\
\
    ret = __ITEM_GET_INT(vm.registers.sp - 1);\
    ck_assert_int_eq(ret, (_x) - (_y));\
\
    Rawcode_Delete(rawcode);\
    halt();\
} while (0)

#define TEST_FSUB(_x, _y) \
do {\
    RawcodeGen *codegen = RawcodeGen_Init();\
    FILE *fp = fopen(SAMPLE_RAWCODE_FILE_NAME, "wb");\
    double ret;\
    struct ins ins[3];\
\
    ins[0].id = ins[1].id = FCONST; \
    ins[2].id = FSUB;\
\
    ins[0].args.fconst.val = (_y);\
    ins[1].args.fconst.val = (_x);\
\
    int i;\
    for (i = 0; i < 3; i++) {\
        RawcodeGen_AddInstruction(codegen, ins[i]);\
    }\
    Rawcode *rawcode = RawcodeGen_Generate(codegen);\
    fwrite(rawcode->rawcode, rawcode->size, 1, fp);\
    fclose(fp);\
\
    load(SAMPLE_RAWCODE_FILE_NAME);\
    execute(SAMPLE_RAWCODE_FILE_NAME);\
\
    ret = __ITEM_GET_FLOAT(vm.registers.sp - 1);\
    ck_assert_float_eq(ret, (_x) - (_y));\
\
    Rawcode_Delete(rawcode);\
    halt();\
} while (0)

#define TEST_IMUL(_x, _y) \
do {\
    RawcodeGen *codegen = RawcodeGen_Init();\
    FILE *fp = fopen(SAMPLE_RAWCODE_FILE_NAME, "wb");\
    double ret;\
    struct ins ins[3];\
\
    ins[0].id = ins[1].id = ICONST; \
    ins[2].id = IMUL;\
\
    ins[0].args.iconst.val = (_y);\
    ins[1].args.iconst.val = (_x);\
\
    int i;\
    for (i = 0; i < 3; i++) {\
        RawcodeGen_AddInstruction(codegen, ins[i]);\
    }\
    Rawcode *rawcode = RawcodeGen_Generate(codegen);\
    fwrite(rawcode->rawcode, rawcode->size, 1, fp);\
    fclose(fp);\
\
    load(SAMPLE_RAWCODE_FILE_NAME);\
    execute(SAMPLE_RAWCODE_FILE_NAME);\
\
    ret = __ITEM_GET_INT(vm.registers.sp - 1);\
    ck_assert_int_eq(ret, (_x) * (_y));\
\
    Rawcode_Delete(rawcode);\
    halt();\
} while (0)

#define TEST_FMUL(_x, _y) \
do {\
    RawcodeGen *codegen = RawcodeGen_Init();\
    FILE *fp = fopen(SAMPLE_RAWCODE_FILE_NAME, "wb");\
    double ret;\
    struct ins ins[3];\
\
    ins[0].id = ins[1].id = FCONST; \
    ins[2].id = FMUL;\
\
    ins[0].args.fconst.val = (_y);\
    ins[1].args.fconst.val = (_x);\
\
    int i;\
    for (i = 0; i < 3; i++) {\
        RawcodeGen_AddInstruction(codegen, ins[i]);\
    }\
    Rawcode *rawcode = RawcodeGen_Generate(codegen);\
    fwrite(rawcode->rawcode, rawcode->size, 1, fp);\
    fclose(fp);\
\
    load(SAMPLE_RAWCODE_FILE_NAME);\
    execute(SAMPLE_RAWCODE_FILE_NAME);\
\
    ret = __ITEM_GET_FLOAT(vm.registers.sp - 1);\
    ck_assert_float_eq(ret, (_x) * (_y));\
\
    Rawcode_Delete(rawcode);\
    halt();\
} while (0)

#define TEST_IDIV(_x, _y) \
do {\
    RawcodeGen *codegen = RawcodeGen_Init();\
    FILE *fp = fopen(SAMPLE_RAWCODE_FILE_NAME, "wb");\
    double ret;\
    struct ins ins[3];\
\
    ins[0].id = ins[1].id = ICONST; \
    ins[2].id = IDIV;\
\
    ins[0].args.iconst.val = (_y);\
    ins[1].args.iconst.val = (_x);\
\
    int i;\
    for (i = 0; i < 3; i++) {\
        RawcodeGen_AddInstruction(codegen, ins[i]);\
    }\
    Rawcode *rawcode = RawcodeGen_Generate(codegen);\
    fwrite(rawcode->rawcode, rawcode->size, 1, fp);\
    fclose(fp);\
\
    load(SAMPLE_RAWCODE_FILE_NAME);\
    execute(SAMPLE_RAWCODE_FILE_NAME);\
\
    ret = __ITEM_GET_INT(vm.registers.sp - 1);\
    ck_assert_int_eq(ret, (_x) / (_y));\
\
    Rawcode_Delete(rawcode);\
    halt();\
} while (0)

#define TEST_IDIV_DIVBYZERO(_x) \
do {\
    RawcodeGen *codegen = RawcodeGen_Init();\
    FILE *fp = fopen(SAMPLE_RAWCODE_FILE_NAME, "wb");\
    double ret;\
    struct ins ins[3];\
\
    ins[0].id = ins[1].id = ICONST; \
    ins[2].id = IDIV;\
\
    ins[0].args.iconst.val = (_x);\
    ins[1].args.iconst.val = 0;\
\
    int i;\
    for (i = 0; i < 3; i++) {\
        RawcodeGen_AddInstruction(codegen, ins[i]);\
    }\
    Rawcode *rawcode = RawcodeGen_Generate(codegen);\
    fwrite(rawcode->rawcode, rawcode->size, 1, fp);\
    fclose(fp);\
\
    load(SAMPLE_RAWCODE_FILE_NAME);\
    execute(SAMPLE_RAWCODE_FILE_NAME);\
\
    ck_assert_int_eq(error_logger.err, INTERNAL_ERROR);\
    ck_assert_str_eq(error_logger.msg, DIVIDED_BY_ZERO);\
\
    Rawcode_Delete(rawcode);\
    halt();\
} while (0)

#define TEST_FDIV(_x, _y) \
do {\
    RawcodeGen *codegen = RawcodeGen_Init();\
    FILE *fp = fopen(SAMPLE_RAWCODE_FILE_NAME, "wb");\
    double ret;\
    struct ins ins[3];\
\
    ins[0].id = ins[1].id = FCONST; \
    ins[2].id = FDIV;\
\
    ins[0].args.fconst.val = (_y);\
    ins[1].args.fconst.val = (_x);\
\
    int i;\
    for (i = 0; i < 3; i++) {\
        RawcodeGen_AddInstruction(codegen, ins[i]);\
    }\
    Rawcode *rawcode = RawcodeGen_Generate(codegen);\
    fwrite(rawcode->rawcode, rawcode->size, 1, fp);\
    fclose(fp);\
\
    load(SAMPLE_RAWCODE_FILE_NAME);\
    execute(SAMPLE_RAWCODE_FILE_NAME);\
\
    ret = __ITEM_GET_FLOAT(vm.registers.sp - 1);\
    ck_assert_float_eq(ret, (_x) / (_y));\
\
    Rawcode_Delete(rawcode);\
    halt();\
} while (0)

#define TEST_MOD(_x, _y) \
do {\
    RawcodeGen *codegen = RawcodeGen_Init();\
    FILE *fp = fopen(SAMPLE_RAWCODE_FILE_NAME, "wb");\
    int ret = 100;\
    struct ins ins[3];\
\
    ins[0].id = ins[1].id = ICONST; \
    ins[2].id = MOD;\
\
    ins[0].args.iconst.val = (_y);\
    ins[1].args.iconst.val = (_x);\
\
    int i;\
    for (i = 0; i < 3; i++) {\
        RawcodeGen_AddInstruction(codegen, ins[i]);\
    }\
    Rawcode *rawcode = RawcodeGen_Generate(codegen);\
    fwrite(rawcode->rawcode, rawcode->size, 1, fp);\
    fclose(fp);\
\
    load(SAMPLE_RAWCODE_FILE_NAME);\
    execute(SAMPLE_RAWCODE_FILE_NAME);\
\
    ret = __ITEM_GET_INT(vm.registers.sp - 1);\
    ck_assert_int_eq(ret, (_x) % (_y));\
\
    Rawcode_Delete(rawcode);\
    halt();\
} while (0)

#define TEST_IEQ(_x, _y) \
do {\
    RawcodeGen *codegen = RawcodeGen_Init();\
    FILE *fp = fopen(SAMPLE_RAWCODE_FILE_NAME, "wb");\
    int ret;\
    struct ins ins[3];\
\
    ins[0].id = ins[1].id = ICONST;\
    ins[2].id = IEQ;\
\
    ins[0].args.iconst.val = (_y);\
    ins[1].args.iconst.val = (_x);\
\
    int i;\
    for (i = 0; i < 3; i++) {\
        RawcodeGen_AddInstruction(codegen, ins[i]);\
    }\
    Rawcode *rawcode = RawcodeGen_Generate(codegen);\
    fwrite(rawcode->rawcode, rawcode->size, 1, fp);\
    fclose(fp);\
\
    load(SAMPLE_RAWCODE_FILE_NAME);\
    execute(SAMPLE_RAWCODE_FILE_NAME);\
\
    ret = __ITEM_GET_INT(vm.registers.sp - 1);\
    ck_assert_int_eq(ret, (_x) == (_y));\
\
    Rawcode_Delete(rawcode);\
    halt();\
} while (0)

#define TEST_FEQ(_x, _y) \
do {\
    RawcodeGen *codegen = RawcodeGen_Init();\
    FILE *fp = fopen(SAMPLE_RAWCODE_FILE_NAME, "wb");\
    int ret;\
    struct ins ins[3];\
\
    ins[0].id = ins[1].id = FCONST;\
    ins[2].id = FEQ;\
\
    ins[0].args.fconst.val = (_y);\
    ins[1].args.fconst.val = (_x);\
\
    int i;\
    for (i = 0; i < 3; i++) {\
        RawcodeGen_AddInstruction(codegen, ins[i]);\
    }\
    Rawcode *rawcode = RawcodeGen_Generate(codegen);\
    fwrite(rawcode->rawcode, rawcode->size, 1, fp);\
    fclose(fp);\
\
    load(SAMPLE_RAWCODE_FILE_NAME);\
    execute(SAMPLE_RAWCODE_FILE_NAME);\
\
    ret = __ITEM_GET_INT(vm.registers.sp - 1);\
    ck_assert_int_eq(ret, (_x) == (_y));\
\
    Rawcode_Delete(rawcode);\
    halt();\
} while (0)

#define TEST_CEQ(_x, _y) \
do {\
    RawcodeGen *codegen = RawcodeGen_Init();\
    FILE *fp = fopen(SAMPLE_RAWCODE_FILE_NAME, "wb");\
    int ret;\
    struct ins ins[3];\
\
    ins[0].id = ins[1].id = CCONST;\
    ins[2].id = CEQ;\
\
    ins[0].args.cconst.val = (_y);\
    ins[1].args.cconst.val = (_x);\
\
    int i;\
    for (i = 0; i < 3; i++) {\
        RawcodeGen_AddInstruction(codegen, ins[i]);\
    }\
    Rawcode *rawcode = RawcodeGen_Generate(codegen);\
    fwrite(rawcode->rawcode, rawcode->size, 1, fp);\
    fclose(fp);\
\
    load(SAMPLE_RAWCODE_FILE_NAME);\
    execute(SAMPLE_RAWCODE_FILE_NAME);\
\
    ret = __ITEM_GET_CHAR(vm.registers.sp - 1);\
    ck_assert_int_eq(ret, (_x) == (_y));\
\
    Rawcode_Delete(rawcode);\
    halt();\
} while (0)

#define TEST_REQ(_x, _y) \
do {\
    RawcodeGen *codegen = RawcodeGen_Init();\
    FILE *fp = fopen(SAMPLE_RAWCODE_FILE_NAME, "wb");\
    int ret;\
    struct ins ins;\
\
    ins.id = IEQ;\
\
    RawcodeGen_AddInstruction(codegen, ins);\
    Rawcode *rawcode = RawcodeGen_Generate(codegen);\
    fwrite(rawcode->rawcode, rawcode->size, 1, fp);\
    fclose(fp);\
\
    load(SAMPLE_RAWCODE_FILE_NAME);\
\
    __ITEM_SET_REF(vm.registers.sp, _y);\
    vm.registers.sp++;\
    __ITEM_SET_REF(vm.registers.sp, _x);\
    vm.registers.sp++;\
\
    execute(SAMPLE_RAWCODE_FILE_NAME);\
\
    ret = __ITEM_GET_INT(vm.registers.sp - 1);\
    ck_assert_int_eq(ret, (_x) == (_y));\
\
    Rawcode_Delete(rawcode);\
    halt();\
} while (0)

#define TEST_ILT(_x, _y) \
do {\
    RawcodeGen *codegen = RawcodeGen_Init();\
    FILE *fp = fopen(SAMPLE_RAWCODE_FILE_NAME, "wb");\
    int ret;\
    struct ins ins[3];\
\
    ins[0].id = ins[1].id = ICONST;\
    ins[2].id = ILT;\
\
    ins[0].args.iconst.val = (_y);\
    ins[1].args.iconst.val = (_x);\
\
    int i;\
    for (i = 0; i < 3; i++) {\
        RawcodeGen_AddInstruction(codegen, ins[i]);\
    }\
    Rawcode *rawcode = RawcodeGen_Generate(codegen);\
    fwrite(rawcode->rawcode, rawcode->size, 1, fp);\
    fclose(fp);\
\
    load(SAMPLE_RAWCODE_FILE_NAME);\
    execute(SAMPLE_RAWCODE_FILE_NAME);\
\
    ret = __ITEM_GET_INT(vm.registers.sp - 1);\
    ck_assert_int_eq(ret, (_x) < (_y));\
\
    Rawcode_Delete(rawcode);\
    halt();\
} while (0)

#define TEST_FLT(_x, _y) \
do {\
    RawcodeGen *codegen = RawcodeGen_Init();\
    FILE *fp = fopen(SAMPLE_RAWCODE_FILE_NAME, "wb");\
    int ret;\
    struct ins ins[3];\
\
    ins[0].id = ins[1].id = FCONST;\
    ins[2].id = FLT;\
\
    ins[0].args.fconst.val = (_y);\
    ins[1].args.fconst.val = (_x);\
\
    int i;\
    for (i = 0; i < 3; i++) {\
        RawcodeGen_AddInstruction(codegen, ins[i]);\
    }\
    Rawcode *rawcode = RawcodeGen_Generate(codegen);\
    fwrite(rawcode->rawcode, rawcode->size, 1, fp);\
    fclose(fp);\
\
    load(SAMPLE_RAWCODE_FILE_NAME);\
    execute(SAMPLE_RAWCODE_FILE_NAME);\
\
    ret = __ITEM_GET_INT(vm.registers.sp - 1);\
    ck_assert_int_eq(ret, (_x) < (_y));\
\
    Rawcode_Delete(rawcode);\
    halt();\
} while (0)

#define TEST_CLT(_x, _y) \
do {\
    RawcodeGen *codegen = RawcodeGen_Init();\
    FILE *fp = fopen(SAMPLE_RAWCODE_FILE_NAME, "wb");\
    int ret;\
    struct ins ins[3];\
\
    ins[0].id = ins[1].id = CCONST;\
    ins[2].id = CLT;\
\
    ins[0].args.cconst.val = (_y);\
    ins[1].args.cconst.val = (_x);\
\
    int i;\
    for (i = 0; i < 3; i++) {\
        RawcodeGen_AddInstruction(codegen, ins[i]);\
    }\
    Rawcode *rawcode = RawcodeGen_Generate(codegen);\
    fwrite(rawcode->rawcode, rawcode->size, 1, fp);\
    fclose(fp);\
\
    load(SAMPLE_RAWCODE_FILE_NAME);\
    execute(SAMPLE_RAWCODE_FILE_NAME);\
\
    ret = __ITEM_GET_CHAR(vm.registers.sp - 1);\
    ck_assert_int_eq(ret, (_x) < (_y));\
\
    Rawcode_Delete(rawcode);\
    halt();\
} while (0)

#define TEST_RLT(_x, _y) \
do {\
    RawcodeGen *codegen = RawcodeGen_Init();\
    FILE *fp = fopen(SAMPLE_RAWCODE_FILE_NAME, "wb");\
    int ret;\
    struct ins ins;\
\
    ins.id = ILT;\
\
    RawcodeGen_AddInstruction(codegen, ins);\
    Rawcode *rawcode = RawcodeGen_Generate(codegen);\
    fwrite(rawcode->rawcode, rawcode->size, 1, fp);\
    fclose(fp);\
\
    load(SAMPLE_RAWCODE_FILE_NAME);\
\
    __ITEM_SET_REF(vm.registers.sp, _y);\
    vm.registers.sp++;\
    __ITEM_SET_REF(vm.registers.sp, _x);\
    vm.registers.sp++;\
\
    execute(SAMPLE_RAWCODE_FILE_NAME);\
\
    ret = __ITEM_GET_INT(vm.registers.sp - 1);\
    ck_assert_int_eq(ret, (_x) < (_y));\
\
    Rawcode_Delete(rawcode);\
    halt();\
} while (0)

#define TEST_NOT(_bval)\
do {\
    RawcodeGen *codegen = RawcodeGen_Init();\
    FILE *fp = fopen(SAMPLE_RAWCODE_FILE_NAME, "wb");\
    int ret;\
    struct ins ins[2];\
    int i;\
\
    ins[0].id = ICONST;\
    ins[1].id = NOT;\
\
    ins[0].args.iconst.val = (_bval);\
\
    for (i = 0; i < 2; i++) {\
        RawcodeGen_AddInstruction(codegen, ins[i]);\
    }\
    Rawcode *rawcode = RawcodeGen_Generate(codegen);\
    fwrite(rawcode->rawcode, rawcode->size, 1, fp);\
    fclose(fp);\
\
    load(SAMPLE_RAWCODE_FILE_NAME);\
    execute(SAMPLE_RAWCODE_FILE_NAME);\
\
    ret = __ITEM_GET_INT(vm.registers.sp - 1);\
    ck_assert_int_eq(ret, !(_bval));\
\
    Rawcode_Delete(rawcode);\
    halt();\
} while (0)

#define TEST_DUP(_val, _constins) \
do {\
    RawcodeGen *codegen = RawcodeGen_Init();\
    FILE *fp = fopen(SAMPLE_RAWCODE_FILE_NAME, "wb");\
    long long int ret1, ret2;\
    struct ins ins[2];\
    int i;\
\
    ins[0].id = (_constins);\
    ins[1].id = DUP;\
\
    ins[0].args.iconst.val = (_val);\
\
    for (i = 0; i < 2; i++) {\
        RawcodeGen_AddInstruction(codegen, ins[i]);\
    }\
    Rawcode *rawcode = RawcodeGen_Generate(codegen);\
    fwrite(rawcode->rawcode, rawcode->size, 1, fp);\
    fclose(fp);\
\
    load(SAMPLE_RAWCODE_FILE_NAME);\
    execute(SAMPLE_RAWCODE_FILE_NAME);\
\
    ret1 = *(vm.registers.sp - 1);\
    ret2 = *(vm.registers.sp - 2);\
\
    ck_assert_int_eq(ret1, ret2);\
\
    Rawcode_Delete(rawcode);\
    halt();\
} while (0)

#define TEST_LOAD(_n, _idx) \
do {\
    RawcodeGen *codegen = RawcodeGen_Init();\
    FILE *fp = fopen(SAMPLE_RAWCODE_FILE_NAME, "wb");\
    long long int ret1, ret2;\
    struct ins ins[(_n) + 2];\
    int i;\
\
    for (i = 0; i < (_n); i++) {\
        ins[i].id = ICONST;\
        ins[i].args.iconst.val = i;\
    }\
    /* index */\
    ins[_n].id = ICONST;\
    ins[_n].args.iconst.val = (_idx);\
    ins[_n + 1].id = LOAD;\
\
    for (i = 0; i < (_n) + 1; i++) {\
        RawcodeGen_AddInstruction(codegen, ins[i]);\
    }\
\
    Rawcode *rawcode = RawcodeGen_Generate(codegen);\
    fwrite(rawcode->rawcode, rawcode->size, 1, fp);\
    fclose(fp);\
\
    load(SAMPLE_RAWCODE_FILE_NAME);\
    execute(SAMPLE_RAWCODE_FILE_NAME);\
\
    ret1 = *(vm.registers.sp - 1);\
    ret2 = vm.stack.stack[_idx];\
\
    ck_assert_int_eq(ret1, ret2);\
\
    Rawcode_Delete(rawcode);\
    halt();\
} while (0)

#define TEST_LOAD_ILLINDEX(_n, _idx) \
do {\
    RawcodeGen *codegen = RawcodeGen_Init();\
    FILE *fp = fopen(SAMPLE_RAWCODE_FILE_NAME, "wb");\
    long long int ret1, ret2;\
    struct ins ins[(_n) + 2];\
    int i;\
\
    for (i = 0; i < (_n); i++) {\
        ins[i].id = ICONST;\
        ins[i].args.iconst.val = i;\
    }\
    /* index */\
    ins[_n].id = ICONST;\
    ins[_n].args.iconst.val = (_idx);\
    ins[_n + 1].id = LOAD;\
\
    for (i = 0; i < (_n) + 2; i++) {\
        RawcodeGen_AddInstruction(codegen, ins[i]);\
    }\
\
    Rawcode *rawcode = RawcodeGen_Generate(codegen);\
    fwrite(rawcode->rawcode, rawcode->size, 1, fp);\
    fclose(fp);\
\
    load(SAMPLE_RAWCODE_FILE_NAME);\
    execute(SAMPLE_RAWCODE_FILE_NAME);\
\
    ck_assert_int_eq(error_logger.err, INTERNAL_ERROR);\
    ck_assert_str_eq(error_logger.msg, ILLEGAL_INDEX);\
\
    Rawcode_Delete(rawcode);\
    halt();\
} while (0)

#define TEST_STORE(_n, _idx, _val) \
do {\
    RawcodeGen *codegen = RawcodeGen_Init();\
    FILE *fp = fopen(SAMPLE_RAWCODE_FILE_NAME, "wb");\
    long long int ret;\
    struct ins ins[(_n) + 3];\
    int i;\
\
    for (i = 0; i < (_n); i++) {\
        ins[i].id = ICONST;\
        ins[i].args.iconst.val = i;\
    }\
    ins[(_n)].id = ICONST;\
    ins[(_n)].args.iconst.val = (_idx);\
    ins[(_n) + 1].id = ICONST;\
    ins[(_n) + 1].args.iconst.val = (_val);\
    ins[(_n) + 2].id = STORE;\
\
    for (i = 0; i < (_n) + 3; i++) {\
        RawcodeGen_AddInstruction(codegen, ins[i]);\
    }\
\
    Rawcode *rawcode = RawcodeGen_Generate(codegen);\
    fwrite(rawcode->rawcode, rawcode->size, 1, fp);\
    fclose(fp);\
\
    load(SAMPLE_RAWCODE_FILE_NAME);\
    execute(SAMPLE_RAWCODE_FILE_NAME);\
\
    ret = __ITEM_GET_INT(vm.stack.stack + (_idx));\
\
    ck_assert_int_eq(ret, _val);\
\
    Rawcode_Delete(rawcode);\
    halt();\
} while (0)

#define TEST_STORE_ILLINDEX(_n, _idx, _val)\
do {\
    RawcodeGen *codegen = RawcodeGen_Init();\
    FILE *fp = fopen(SAMPLE_RAWCODE_FILE_NAME, "wb");\
    long long int ret;\
    struct ins ins[(_n) + 3];\
    int i;\
\
    for (i = 0; i < (_n); i++) {\
        ins[i].id = ICONST;\
        ins[i].args.iconst.val = i;\
    }\
    ins[(_n)].id = ICONST;\
    ins[(_n)].args.iconst.val = (_idx);\
    ins[(_n) + 1].id = ICONST;\
    ins[(_n) + 1].args.iconst.val = (_val);\
    ins[(_n) + 2].id = STORE;\
\
    for (i = 0; i < (_n) + 3; i++) {\
        RawcodeGen_AddInstruction(codegen, ins[i]);\
    }\
\
    Rawcode *rawcode = RawcodeGen_Generate(codegen);\
    fwrite(rawcode->rawcode, rawcode->size, 1, fp);\
    fclose(fp);\
\
    load(SAMPLE_RAWCODE_FILE_NAME);\
    execute(SAMPLE_RAWCODE_FILE_NAME);\
\
    ret = vm.stack.stack[_idx];\
\
    ck_assert_int_eq(error_logger.err, INTERNAL_ERROR);\
    ck_assert_str_eq(error_logger.msg, ILLEGAL_INDEX);\
\
    Rawcode_Delete(rawcode);\
    halt();\
} while (0)

#define TEST_ALTSP(_origin, _m) \
do {\
    RawcodeGen *codegen = RawcodeGen_Init();\
    FILE *fp = fopen(SAMPLE_RAWCODE_FILE_NAME, "wb");\
    long long int ret;\
    struct ins ins;\
\
    ins.id = ALTSP;\
    ins.args.altsp.m = (_m);\
\
    RawcodeGen_AddInstruction(codegen, ins);\
\
    Rawcode *rawcode = RawcodeGen_Generate(codegen);\
    fwrite(rawcode->rawcode, rawcode->size, 1, fp);\
    fclose(fp);\
\
    load(SAMPLE_RAWCODE_FILE_NAME);\
\
    vm.registers.sp += (_origin);\
    execute(SAMPLE_RAWCODE_FILE_NAME);\
\
    ck_assert_ptr_eq(vm.stack.stack + (_origin) + (_m), vm.registers.sp);\
\
    Rawcode_Delete(rawcode);\
   halt();\
}  while (0)

#define TEST_ALTSP_ILLARGS(_origin, _m) \
do {\
    RawcodeGen *codegen = RawcodeGen_Init();\
    FILE *fp = fopen(SAMPLE_RAWCODE_FILE_NAME, "wb");\
    long long int ret;\
    struct ins ins;\
\
    ins.id = ALTSP;\
    ins.args.altsp.m = (_m);\
\
    RawcodeGen_AddInstruction(codegen, ins);\
\
    Rawcode *rawcode = RawcodeGen_Generate(codegen);\
    fwrite(rawcode->rawcode, rawcode->size, 1, fp);\
    fclose(fp);\
\
    vm.registers.sp = vm.stack.stack + (_origin);\
\
    load(SAMPLE_RAWCODE_FILE_NAME);\
    execute(SAMPLE_RAWCODE_FILE_NAME);\
\
    ck_assert_int_eq(error_logger.err, INTERNAL_ERROR);\
    ck_assert_str_eq(error_logger.msg, ILLEGAL_ALTSP_ARGS);\
\
    Rawcode_Delete(rawcode);\
    halt();\
} while (0)

/* generate a sequence of rawcode with format:
 * jump (_addr)
 * iconst 1 
 * iconst 2
 * ...
 * iconst (_inslen)
 */
#define TEST_JUMP(_inslen, _addr)\
do {\
    RawcodeGen *codegen = RawcodeGen_Init();\
    FILE *fp = fopen(SAMPLE_RAWCODE_FILE_NAME, "wb");\
    long long int ret;\
    struct ins ins[(_inslen) + 1];\
    int i;\
\
    ins[0].id = JUMP;\
    ins[1].args.jxxx.addr = (_addr);\
    for (i = 1; i <= (_inslen); i++) {\
        ins[i].id = ICONST;\
        ins[i].args.iconst.val = i;\
    }\
\
    for (i = 0; i < (_inslen) + 1; i++) {\
        RawcodeGen_AddInstruction(codegen, ins[i]);\
    }\
\
    Rawcode *rawcode = RawcodeGen_Generate(codegen);\
    fwrite(rawcode->rawcode, rawcode->size, 1, fp);\
    fclose(fp);\
\
    load(SAMPLE_RAWCODE_FILE_NAME);\
\
    vm.instructions.length = (_inslen);\
    execute(SAMPLE_RAWCODE_FILE_NAME);\
\
    ck_assert_ptr_eq(vm.registers.pc, vm.instructions.inslist + ins[0].args.jxxx.addr);\
\
    Rawcode_Delete(rawcode);\
    halt();\
} while (0)

#define TEST_JUMP_ILLARGS(_inslen, _addr)\
do {\
    RawcodeGen *codegen = RawcodeGen_Init();\
    FILE *fp = fopen(SAMPLE_RAWCODE_FILE_NAME, "wb");\
    long long int ret;\
    struct ins ins[(_inslen) + 1];\
    int i;\
\
    ins[0].id = JUMP;\
    ins[1].args.jxxx.addr = (_addr);\
    for (i = 1; i <= (_inslen); i++) {\
        ins[i].id = ICONST;\
        ins[i].args.iconst.val = i;\
    }\
\
    for (i = 0; i < (_inslen) + 1; i++) {\
        RawcodeGen_AddInstruction(codegen, ins[i]);\
    }\
\
    Rawcode *rawcode = RawcodeGen_Generate(codegen);\
    fwrite(rawcode->rawcode, rawcode->size, 1, fp);\
    fclose(fp);\
\
    load(SAMPLE_RAWCODE_FILE_NAME);\
    vm.instructions.length = (_inslen);\
    execute(SAMPLE_RAWCODE_FILE_NAME);\
\
    ck_assert_int_eq(error_logger.err, INTERNAL_ERROR);\
    ck_assert_str_eq(error_logger.msg, ILLEGAL_JUMPING_ADDR);\
\
    Rawcode_Delete(rawcode);\
    halt();\
} while (0)

#define TEST_CALL(_m, _a)\
do {\
    RawcodeGen *codegen = RawcodeGen_Init();\
    FILE *fp = fopen(SAMPLE_RAWCODE_FILE_NAME, "wb");\
    long long int ret;\
    struct ins ins[(_inslen) + 1];\
    int i;\
\
    ins[0].id = CALL;\
    ins[1].args.jxxx.addr = (_addr);\
    for (i = 1; i <= (_inslen); i++) {\
        ins[i].id = ICONST;\
        ins[i].args.iconst.val = i;\
    }\
\
    for (i = 0; i < (_inslen) + 1; i++) {\
        RawcodeGen_AddInstruction(codegen, ins[i]);\
    }\
\
    Rawcode *rawcode = RawcodeGen_Generate(codegen);\
    fwrite(rawcode->rawcode, rawcode->size, 1, fp);\
    fclose(fp);\
\
    load(SAMPLE_RAWCODE_FILE_NAME);\
    vm.instructions.length = (_inslen);\
    execute(SAMPLE_RAWCODE_FILE_NAME);\
\
    ck_assert_int_eq(error_logger.err, INTERNAL_ERROR);\
    ck_assert_str_eq(error_logger.msg, ILLEGAL_JUMPING_ADDR);\
\
    Rawcode_Delete(rawcode);\
    halt();\
} while (0)

START_TEST(test_iconst_1) 
{
    TEST_ICONST(10);
}
END_TEST

START_TEST(test_iconst_2) 
{
    TEST_ICONST(-10);
}
END_TEST

START_TEST(test_iconst_3) 
{
    TEST_ICONST(0);
}
END_TEST

START_TEST(test_fconst_1) 
{
    TEST_FCONST(0.0);
}
END_TEST

START_TEST(test_fconst_2) 
{
    TEST_FCONST(1E30);
}
END_TEST

START_TEST(test_fconst_3) 
{
    TEST_FCONST(-1E30);
}
END_TEST

START_TEST(test_cconst_1)
{
    TEST_CCONST('a');
}
END_TEST

START_TEST(test_cconst_2)
{
    TEST_CCONST('\t');
}
END_TEST

START_TEST(test_cconst_3)
{
    TEST_CCONST('\0');
}
END_TEST

START_TEST(test_iadd_1)
{
    TEST_IADD(100, 10);
}
END_TEST

START_TEST(test_iadd_2)
{
    TEST_IADD(-100, 10);
}
END_TEST

START_TEST(test_iadd_3)
{
    TEST_IADD(1 << 20, 1 << 10);
}
END_TEST

START_TEST(test_iadd_4)
{
    TEST_IADD(1 << 20, -(1 << 10));
}
END_TEST

START_TEST(test_isub_1)
{
    TEST_ISUB(100, 10);
}
END_TEST

START_TEST(test_isub_2)
{
    TEST_ISUB(-100, 10);
}
END_TEST

START_TEST(test_isub_3)
{
    TEST_ISUB(1 << 20, 1 << 10);
}
END_TEST

START_TEST(test_isub_4)
{
    TEST_ISUB(1 << 20, -(1 << 10));
}
END_TEST

START_TEST(test_fadd_1)
{
    TEST_FADD(100.0, 10.0);
}
END_TEST

START_TEST(test_fadd_2)
{
    TEST_FADD(-100.0, 10.0);
}
END_TEST

START_TEST(test_fadd_3)
{
    TEST_FADD(1.341E20, 1.341E12);
}
END_TEST

START_TEST(test_fadd_4)
{
    TEST_FADD(-1.341E20, 1.341E12);
}
END_TEST

START_TEST(test_fsub_1)
{
    TEST_FSUB(100.0, 10.0);
}
END_TEST

START_TEST(test_fsub_2)
{
    TEST_FSUB(-100.0, 10.0);
}
END_TEST

START_TEST(test_fsub_3)
{
    TEST_FSUB(1.341E20, 1.341E12);
}
END_TEST

START_TEST(test_fsub_4)
{
    TEST_FSUB(-1.341E20, 1.341E12);
}
END_TEST

START_TEST(test_imul_1)
{
    TEST_IMUL(10, 100);
}
END_TEST

START_TEST(test_imul_2)
{
    TEST_IMUL(-10, 100);
}
END_TEST

START_TEST(test_imul_3)
{
    TEST_IMUL(10, -100);
}
END_TEST

START_TEST(test_imul_4)
{
    TEST_IMUL(-10, -100);
}
END_TEST

START_TEST(test_fmul_1)
{
    TEST_FMUL(10.134, 100.345);
}
END_TEST

START_TEST(test_fmul_2)
{
    TEST_FMUL(-10.134, 100.345);
}
END_TEST

START_TEST(test_fmul_3)
{
    TEST_FMUL(10.134, -100.345);
}
END_TEST

START_TEST(test_fmul_4)
{
    TEST_FMUL(-10.134, -100.345);
}
END_TEST

START_TEST(test_idiv_1)
{
    TEST_IDIV(10, 100);
}
END_TEST

START_TEST(test_idiv_2)
{
    TEST_IDIV(-10, 100);
}
END_TEST

START_TEST(test_idiv_3)
{
    TEST_IDIV(10, -100);
}
END_TEST

START_TEST(test_idiv_4)
{
    TEST_IDIV(-10, -100);
}
END_TEST

START_TEST(test_idiv_divbyzero)
{
    TEST_IDIV_DIVBYZERO(34);
}
END_TEST

START_TEST(test_fdiv_1)
{
    TEST_FDIV(10.134, 100.345);
}
END_TEST

START_TEST(test_fdiv_2)
{
    TEST_FDIV(-10.134, 100.345);
}
END_TEST

START_TEST(test_fdiv_3)
{
    TEST_FDIV(10.134, -100.345);
}
END_TEST

START_TEST(test_fdiv_4)
{
    TEST_FDIV(-10.134, -100.345);
}
END_TEST

START_TEST(test_mod_1)
{
    TEST_MOD(12, 5);
}
END_TEST

START_TEST(test_mod_2)
{
    TEST_MOD(12, 30);
}
END_TEST

START_TEST(test_mod_3)
{
    TEST_MOD(3, 7);
}
END_TEST

START_TEST(test_ieq_1)
{
    TEST_IEQ(3, 7);
}
END_TEST

START_TEST(test_ieq_2)
{
    TEST_IEQ(304, 304);
}
END_TEST

START_TEST(test_feq_1)
{
    TEST_FEQ(3.0, 7.0);
}
END_TEST

START_TEST(test_feq_2)
{
    TEST_FEQ(3345.3434, 3345.3434);
}
END_TEST

START_TEST(test_req_1)
{
    TEST_REQ(0x1234567890abcde0, 0x1234567890acdef1);
}
END_TEST

START_TEST(test_req_2)
{
    TEST_REQ(0x1234567890abcde0, 0x1234567890abcde0);
}
END_TEST


START_TEST(test_ceq_1)
{
    TEST_CEQ('a', 'c');
}
END_TEST

START_TEST(test_ceq_2)
{
    TEST_CEQ('a', 'a');
}
END_TEST

START_TEST(test_ilt_1)
{
    TEST_ILT(4, 10);
}
END_TEST

START_TEST(test_ilt_2)
{
    TEST_ILT(10, 10);
}
END_TEST

START_TEST(test_flt_1)
{
    TEST_FLT(-13.345, 4.13);
}
END_TEST

START_TEST(test_flt_2)
{
    TEST_FLT(35.133, 35.133);
}
END_TEST

START_TEST(test_clt_1)
{
    TEST_CLT('a', '\t');
}
END_TEST

START_TEST(test_clt_2)
{
    TEST_CLT('A', 'A');
}
END_TEST

START_TEST(test_rlt_1)
{
    TEST_RLT(0X123, 0X199);
}
END_TEST

START_TEST(test_rlt_2)
{
    TEST_RLT(0X32554514321, 0X32554514321);
}
END_TEST

START_TEST(test_not_1)
{
    TEST_NOT(243);
}
END_TEST

START_TEST(test_not_2)
{
    TEST_NOT(0);
}
END_TEST

START_TEST(test_dup_1)
{
    TEST_DUP(3214, ICONST);
}
END_TEST

START_TEST(test_dup_2)
{
    TEST_DUP(334.3431, FCONST);
}
END_TEST


START_TEST(test_dup_3)
{
    TEST_DUP(343.133, FCONST);
}
END_TEST

START_TEST(test_dup_4)
{
    TEST_DUP('Z', CCONST);
}
END_TEST

START_TEST(test_load_1)
{
    TEST_LOAD(10, 1);
}
END_TEST

START_TEST(test_load_2)
{
    TEST_LOAD(4, 4);
}
END_TEST

START_TEST(test_load_3)
{
    TEST_LOAD(100, 30);
}
END_TEST

START_TEST(test_loaderr_1)
{
    TEST_LOAD_ILLINDEX(10, 30);
}
END_TEST

START_TEST(test_loaderr_2)
{
    TEST_LOAD_ILLINDEX(10, -30);
}
END_TEST

START_TEST(test_store_1)
{
    TEST_STORE(100, 0, 130);
}
END_TEST

START_TEST(test_store_2)
{
    TEST_STORE(100, 99, 130);
}
END_TEST

START_TEST(test_store_3)
{
    TEST_STORE(100, 50, 130);
}
END_TEST

START_TEST(test_storeerr_1)
{
    TEST_STORE_ILLINDEX(100, -1, 0);
}
END_TEST

START_TEST(test_storeerr_2)
{
    TEST_STORE_ILLINDEX(100, 101, 0);
}
END_TEST

START_TEST(test_altsp_1)
{
    TEST_ALTSP(10, -3);
}
END_TEST

START_TEST(test_altsp_2)
{
    TEST_ALTSP(10, 3);
}
END_TEST

START_TEST(test_altsp_3)
{
    TEST_ALTSP(10, 0);
}
END_TEST

START_TEST(test_altsperr_1)
{
    TEST_ALTSP_ILLARGS(10, 100);
}
END_TEST

START_TEST(test_altsperr_2)
{
    TEST_ALTSP_ILLARGS(10, -100);
}
END_TEST

START_TEST(test_getsp)
{
    RawcodeGen *codegen = RawcodeGen_Init();
    FILE *fp = fopen(SAMPLE_RAWCODE_FILE_NAME, "wb");
    double ret;
    struct ins ins;

    ins.id = GETSP;
    RawcodeGen_AddInstruction(codegen, ins);
    Rawcode *rawcode = RawcodeGen_Generate(codegen);
    fwrite(rawcode->rawcode, rawcode->size, 1, fp);
    fclose(fp);

    load(SAMPLE_RAWCODE_FILE_NAME);
    execute(SAMPLE_RAWCODE_FILE_NAME);

    ck_assert_int_eq(__ITEM_GET_INT(vm.registers.sp - 1), (int)(long long int)(vm.registers.sp - 1));

    Rawcode_Delete(rawcode);
    halt();
}
END_TEST

START_TEST(test_getbp)
{
    RawcodeGen *codegen = RawcodeGen_Init();
    FILE *fp = fopen(SAMPLE_RAWCODE_FILE_NAME, "wb");
    double ret;
    struct ins ins;

    ins.id = GETBP;
    RawcodeGen_AddInstruction(codegen, ins);
    Rawcode *rawcode = RawcodeGen_Generate(codegen);
    fwrite(rawcode->rawcode, rawcode->size, 1, fp);
    fclose(fp);

    load(SAMPLE_RAWCODE_FILE_NAME);
    execute(SAMPLE_RAWCODE_FILE_NAME);

    ck_assert_int_eq(__ITEM_GET_INT(vm.registers.sp - 1), (int)(long long int)vm.registers.bp);

    Rawcode_Delete(rawcode);
    halt();
}
END_TEST

/* cannot test, need to adjust interface */
/*
START_TEST(test_jump_1)
{
    TEST_JUMP(120, 50);
}
END_TEST

START_TEST(test_jump_2)
{
    TEST_JUMP(120, 10);
}
END_TEST

START_TEST(test_jump_3)
{
    TEST_JUMP(120, 119);
}
END_TEST

START_TEST(test_jumperr_1)
{
    TEST_JUMP_ILLARGS(120, -1);
}
END_TEST

START_TEST(test_jumperr_2)
{
    TEST_JUMP_ILLARGS(120, 120);
}
END_TEST

START_TEST(test_jumperr_3)
{
    TEST_JUMP_ILLARGS(120, 200);
}
END_TEST
*/
Suite *test_ins() {

    Suite *s = suite_create("Test Instructions");

    TCase *test_xconst = tcase_create("Test IConst Instruction");;

    tcase_add_test(test_xconst, test_iconst_1);
    tcase_add_test(test_xconst, test_iconst_2);
    tcase_add_test(test_xconst, test_iconst_3);

    tcase_add_test(test_xconst, test_fconst_1);
    tcase_add_test(test_xconst, test_fconst_2);
    tcase_add_test(test_xconst, test_fconst_3);

    tcase_add_test(test_xconst, test_cconst_1);
    tcase_add_test(test_xconst, test_cconst_2);
    tcase_add_test(test_xconst, test_cconst_3);

    TCase *test_add = tcase_create("Test Add Instrution");
    tcase_add_test(test_add, test_iadd_1);
    tcase_add_test(test_add, test_iadd_2);
    tcase_add_test(test_add, test_iadd_3);
    tcase_add_test(test_add, test_iadd_4);
    tcase_add_test(test_add, test_fadd_1);
    tcase_add_test(test_add, test_fadd_2);
    tcase_add_test(test_add, test_fadd_3);
    tcase_add_test(test_add, test_fadd_4);

    TCase *test_sub = tcase_create("Test Add Instrution");
    tcase_add_test(test_sub, test_isub_1);
    tcase_add_test(test_sub, test_isub_2);
    tcase_add_test(test_sub, test_isub_3);
    tcase_add_test(test_sub, test_isub_4);
    tcase_add_test(test_sub, test_fsub_1);
    tcase_add_test(test_sub, test_fsub_2);
    tcase_add_test(test_sub, test_fsub_3);
    tcase_add_test(test_sub, test_fsub_4);

    TCase *test_mul = tcase_create("Test Mul Instruction");
    tcase_add_test(test_mul, test_imul_1);
    tcase_add_test(test_mul, test_imul_2);
    tcase_add_test(test_mul, test_imul_3);
    tcase_add_test(test_mul, test_imul_4);

    tcase_add_test(test_mul, test_fmul_1);
    tcase_add_test(test_mul, test_fmul_2);
    tcase_add_test(test_mul, test_fmul_3);
    tcase_add_test(test_mul, test_fmul_4);
    

    TCase *test_div = tcase_create("Test Div Instruction");
    tcase_add_test(test_div, test_idiv_1);
    tcase_add_test(test_div, test_idiv_2);
    tcase_add_test(test_div, test_idiv_3);
    tcase_add_test(test_div, test_idiv_4);

    tcase_add_test(test_div, test_idiv_divbyzero);

    tcase_add_test(test_div, test_fdiv_1);
    tcase_add_test(test_div, test_fdiv_2);
    tcase_add_test(test_div, test_fdiv_3);
    tcase_add_test(test_div, test_fdiv_4);

    TCase *test_mod = tcase_create("Test Mod Instruction");
    tcase_add_test(test_mod, test_mod_1);
    tcase_add_test(test_mod, test_mod_2);
    tcase_add_test(test_mod, test_mod_3);

    TCase *test_xeq = tcase_create("Test Equal Instruction");
    tcase_add_test(test_xeq, test_ieq_1);
    tcase_add_test(test_xeq, test_ieq_2);
    tcase_add_test(test_xeq, test_feq_1);
    tcase_add_test(test_xeq, test_feq_2);
    tcase_add_test(test_xeq, test_ceq_1);
    tcase_add_test(test_xeq, test_ceq_2);
    tcase_add_test(test_xeq, test_req_1);
    tcase_add_test(test_xeq, test_req_2);

    TCase *test_xlt = tcase_create("Test Equal Instruction");
    tcase_add_test(test_xlt, test_ilt_1);
    tcase_add_test(test_xlt, test_ilt_2);
    tcase_add_test(test_xlt, test_flt_1);
    tcase_add_test(test_xlt, test_flt_2);
    tcase_add_test(test_xlt, test_clt_1);
    tcase_add_test(test_xlt, test_clt_2);
    tcase_add_test(test_xlt, test_rlt_1);
    tcase_add_test(test_xlt, test_rlt_2);

    TCase *test_not = tcase_create("Test Not Instruction");
    tcase_add_test(test_not, test_not_1);   
    tcase_add_test(test_not, test_not_2);   

    TCase *test_stkacc = tcase_create("Test Stack Accesss Instruction: Dup, Store, Load");

    tcase_add_test(test_stkacc, test_dup_1); 
    tcase_add_test(test_stkacc, test_dup_2); 
    tcase_add_test(test_stkacc, test_dup_3); 
    tcase_add_test(test_stkacc, test_dup_4); 
    tcase_add_test(test_stkacc, test_load_1); 
    tcase_add_test(test_stkacc, test_load_2); 
    tcase_add_test(test_stkacc, test_load_3); 
    tcase_add_test(test_stkacc, test_loaderr_1); 
    tcase_add_test(test_stkacc, test_loaderr_2); 
    tcase_add_test(test_stkacc, test_store_1); 
    tcase_add_test(test_stkacc, test_store_2); 
    tcase_add_test(test_stkacc, test_store_3); 
    tcase_add_test(test_stkacc, test_storeerr_1); 
    tcase_add_test(test_stkacc, test_storeerr_2); 

    TCase *test_regacc = tcase_create("Test Registers Access Instruction");
    tcase_add_test(test_regacc, test_getsp);
    tcase_add_test(test_regacc, test_getbp);
    tcase_add_test(test_regacc, test_altsp_1);
    tcase_add_test(test_regacc, test_altsp_2);
    tcase_add_test(test_regacc, test_altsp_3);
    tcase_add_test(test_regacc, test_altsperr_1);
    tcase_add_test(test_regacc, test_altsperr_2);

    suite_add_tcase(s, test_xconst);
    suite_add_tcase(s, test_add);
    suite_add_tcase(s, test_sub);
    suite_add_tcase(s, test_mul);
    suite_add_tcase(s, test_div);
    suite_add_tcase(s, test_mod);
    suite_add_tcase(s, test_xeq);
    suite_add_tcase(s, test_xlt);
    suite_add_tcase(s, test_not);
    suite_add_tcase(s, test_stkacc);
    suite_add_tcase(s, test_regacc);

    return s;
}

int main() {
    int number_failed;
    
    Suite *s = test_ins();
    
    SRunner *sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);

    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
