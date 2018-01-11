#include <stdio.h>
#include <stdlib.h>
#include <check.h>

#include "rawcode.h"
#include "vm.h"
#include "errlog.h"
#include "cJSON.h"
#include "positions.h"

#define SAMPLE_RAWCODE_FILE_NAME "sample_rawocde.pbc"

START_TEST(test_iconst_1) 
{
    RawcodeGen *codegen = RawcodeGen_Init();
    FILE *fp = fopen(SAMPLE_RAWCODE_FILE_NAME, "wb");
 
    struct ins ins;
    ins.id = ICONST;
    ins.args.iconst.val = 10; 

    RawcodeGen_AddInstruct(codegen, ins);
    Rawcode *rawcode = RawcodeGen_Generate(codegen);

    fwrite(rawcode->rawcode, rawcode->size, 1, fp);

    load(SAMPLE_RAWCODE_FILE_NAME);
    execute();

    int ret = *((int *)(vm.registers.sp - sizeof(int)));
    ck_assert_int_eq(ret, 10);

    free(rawcode);
    fclose(fp);
}
END_TEST

Suite *test_ins() {
    Suite *s = suite_create("Test Instructions");

    TCase *test_iconst = tcase_create("Test IConst Instruction");;

    test_add_test(test_iconst, test_iconst_1);

    suite_add_tcase(s, test_iconst);

    return s;
}

int main() {
    int number_failed;
    
    Suite *s = test_ins();
    
    SRunner *sr = srunner_create(s);

    srunner_run_add(sr, CK_NORMAL);

    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
