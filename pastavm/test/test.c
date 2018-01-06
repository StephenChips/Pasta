#include <stdio.h>
#include <stdlib.h>
#include <check.h>
#include "vm.h"
#include "errlog.h"
#include "cJSON.h"

#define SAMPLE_RAWCODE_FILE_NAME "sample_bytecode.pbc"

#define TEST_HEAP_CAPACITY DEFAULT_HEAP_CAPACITY
#define TEST_STACK_CAPACITY DEFAULT_STACK_CAPACITY

/* NOTE: 
 * WHEN RAWCODE FILE JUST LOADED, FOLLOWING SHOULD BE HOLD:
 * 1. no exception handler is set, so the `hr` register should be null
 * 2. no stack frame is added, so register bp should be null too
 * 3. stack pointer register `sp` should point to the base of the stack
 * 4. program counter register `pc` should point to the start of the instruction list
 */
#define CHECK_REG(pcval) \
do { \
    ck_assert_ptr_null(vm.registers.hr); \
    ck_assert_ptr_null(vm.registers.bp); \
    ck_assert_ptr_eq(vm.registers.sp, vm.stack.stack); \
    ck_assert_ptr_eq(vm.registers.pc, vm.instructions.inslist); \
} while (0)

/* NOTE: 
 * WHEN RAWCODE FILE JUST LOADED, FOLLOWING SHOULD BE HOLD:
 * 1. The heap is empty. No item is registered. So the linked list of heap item should be null, 
 *    and the varible to record current heap size should be set to zero.
 * 2. The variable stores heap capacity, which indicates the max size of the heap, should be 
      set as configure file discribed, in here is DEFAULT_HEAP_CAPACITY
 * 3. The survive flag for GC program is set to DEFAULT_SURVIVE_FLAG
 */
#define CHECK_HEAP() \
do {\
    ck_assert_ptr_null(vm.heap.list); \
    ck_assert_int_eq(vm.heap.capacity, TEST_HEAP_CAPACITY); \
    ck_assert_int_eq(vm.heap.survive_flag, GC_DEFAULT_SURVIVE_FLAG); \
    ck_assert_int_eq(vm.heap.current_size, 0); \
} while (0) 

#define CHECK_STACK() \
do { \
    ck_assert_int_eq(vm.stack.capacity,  TEST_STACK_CAPACITY); \
    ck_assert_ptr_nonnull(vm.stack.stack); \
} while (0)

#define CHECK_RAWCODE() \
do { \
    ck_assert_int_eq(vm.constant_pool.count, __CST_COUNT(vm.rawcode)); \
    ck_assert_int_eq(vm.instructions.length, __INS_LENGTH(vm.rawcode)); \
    ck_assert_ptr_eq(vm.constant_pool.positions, __ARRAY_OF_PTR_TO_CONSTANTS(vm.rawcode)); \
    ck_assert_ptr_eq(vm.instructions.inslist, __ARRAY_OF_INSTRUCTION(vm.rawcode)); \
} while (0) 

/* The Index and real data is ommited */
struct sample_rawcode {
    unsigned long int offset_to_cstpool, offset_to_ins;
    unsigned long int constant_count, instruction_len;
};

void setup_sample_config();
void teardown_sample_config();
void setup_sample_rawcode_file(unsigned long int constant_count, unsigned long int instruction_len);
void teardown_sample_rawcode_file();

void setup_sample_rawcode_file(unsigned long int constant_count, unsigned long int instruction_len) {

    struct sample_rawcode sample;
    FILE *fp = NULL;
    if ((fp = fopen(SAMPLE_RAWCODE_FILE_NAME, "wb")) == NULL) {
        printf("%s\n",  "Cannot create sample rawcode file. Please try running the test again.");
        exit(EXIT_FAILURE);
    }

    sample.offset_to_cstpool = sizeof(unsigned long int) * 2;
    sample.offset_to_ins = sizeof(unsigned long int) * 3;
    sample.constant_count = constant_count;
    sample.instruction_len = instruction_len;

    fwrite(&sample, sizeof(struct sample_rawcode), 1, fp);

    fclose(fp);
}


void teardown_sample_rawcode_file() { 
    remove(SAMPLE_RAWCODE_FILE_NAME);    
}

void setup_sample_config() {
    
    char *json = NULL;
    FILE *fp = NULL;
    cJSON *conf, *stack_capacity, *heap_capacity;
    
    if ((fp =fopen(__CFNAME_CONF_FILE_NAME, "w")) == NULL) {
        printf("%s\n", "Cannot create sample config file. Please try running the test again.");
        exit(EXIT_FAILURE);
    }

    conf = cJSON_CreateObject();
    heap_capacity = cJSON_CreateNumber(TEST_HEAP_CAPACITY);
    stack_capacity = cJSON_CreateNumber(TEST_STACK_CAPACITY);
    cJSON_AddItemToObject(conf, __CFNAME_HEAP_CAPACITY, heap_capacity);
    cJSON_AddItemToObject(conf, __CFNAME_STACK_CAPACITY, stack_capacity);

    json = cJSON_Print(conf);
    
    fputs(json, fp);
    
    free(json);
    cJSON_Delete(conf);

    fclose(fp);
}

void teardown_sample_config() {
    remove(__CFNAME_CONF_FILE_NAME);
}

START_TEST(test_load_rawcode_1)
{
    unsigned long int constant_number, ins_area_length;

    setup_sample_rawcode_file(constant_number = 10, ins_area_length = 100);
    load(SAMPLE_RAWCODE_FILE_NAME);

    CHECK_REG(); 
    CHECK_HEAP();
    CHECK_STACK();
    CHECK_RAWCODE();

    ck_assert_int_eq(error_logger.err, NO_ERROR);
    ck_assert_str_eq(error_logger.msg, OK);
    teardown_sample_rawcode_file(); 
}
END_TEST

START_TEST(test_load_rawcode_2)
{
    unsigned long int constant_number, ins_area_length;

    setup_sample_rawcode_file(constant_number = 100, ins_area_length = 100000);
    load(SAMPLE_RAWCODE_FILE_NAME);

    CHECK_REG(); 
    CHECK_HEAP();
    CHECK_STACK();
    CHECK_RAWCODE();

    teardown_sample_rawcode_file(); 
}
END_TEST

START_TEST(test_load_rawcode_3)
{
    unsigned long int constant_number, ins_area_length;

    setup_sample_rawcode_file(constant_number = 1 << 32 - 1, ins_area_length = 1 << 32 - 1);
    load(SAMPLE_RAWCODE_FILE_NAME);

    CHECK_REG(); 
    CHECK_HEAP();
    CHECK_STACK();
    CHECK_RAWCODE();

    teardown_sample_rawcode_file(); 
}
END_TEST

START_TEST(test_load_rawcode_4)
{
    unsigned long int constant_number, ins_area_length;

    setup_sample_rawcode_file(constant_number = 0, ins_area_length = 10);
    load(SAMPLE_RAWCODE_FILE_NAME);

    CHECK_REG(); 
    CHECK_HEAP();
    CHECK_STACK();
    CHECK_RAWCODE();

    teardown_sample_rawcode_file(); 
}
END_TEST

START_TEST(test_load_rawcode_5)
{
    unsigned long int constant_number, ins_area_length;

    setup_sample_rawcode_file(constant_number = 1, ins_area_length = 1);
    load(SAMPLE_RAWCODE_FILE_NAME);

    CHECK_REG(); 
    CHECK_HEAP();
    CHECK_STACK();
    CHECK_RAWCODE();

    teardown_sample_rawcode_file(); 
}
END_TEST

START_TEST(test_load_rawcode_6)
{
    unsigned long int constant_number, ins_area_length;

    setup_sample_rawcode_file(constant_number = 1 << 32 - 10, ins_area_length = 1 << 32 - 10);
    load(SAMPLE_RAWCODE_FILE_NAME);

    CHECK_REG(); 
    CHECK_HEAP();
    CHECK_STACK();
    CHECK_RAWCODE();

    teardown_sample_rawcode_file(); 
}
END_TEST

/* TEST OF VM LOAD
 *
 * DESCRIPTION:
 * Giving a fixed config file and a fake bytecode file, and test loading and initializing VM
 * Check
 * 1. If Registers value set to correct value
 * 2. If heap and stack initialized properly
 * 3. If corresponding pointers points to right position of rawcode
 */
Suite *test_vm_load() {
    Suite *s;
    TCase *tc_core;

    s = suite_create("LoadVM");

    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_load_rawcode_1);
    tcase_add_test(tc_core, test_load_rawcode_2);
    tcase_add_test(tc_core, test_load_rawcode_3);
    tcase_add_test(tc_core, test_load_rawcode_4);
    tcase_add_test(tc_core, test_load_rawcode_5);
    tcase_add_test(tc_core, test_load_rawcode_6);
    suite_add_tcase(s, tc_core);

    return s;
}


int main() {
    
    int number_failed;
    Suite *s;
    SRunner *sr;

    setup_sample_config();

    s = test_vm_load();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);

    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

//    teardown_sample_config();
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
