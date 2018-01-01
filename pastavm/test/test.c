#include <check.h>
#include "vm.h"

#define SAMPLE_BYTECODE_SIZE (2 * sizeof(char *) + sizeof(unsigned long int) + sizeof(unsigned long int))

#define TEST_STK_CAPACITY DEFAULT_STACK_CAPACITY
#define TEST_HEAP_CAPACITY DEFAULT_HEAP_CAPACITY

#define CSTPOOL_START_POS (((void **)rawcode)[0])
#define SAMPLE_RAWCODE_INSTR_START_POOL (((void **)rawcode)[1])
#define SAMPLE_RAWCODE_CSTCOUN 

#define CHECK_CONF() \
do { \
    ck_assert_int_eq(vm.stack.capacity, TEST_STACK_CAPACITY); \
    ck_assert_int_eq(vm.heap.capacity,  TEST_HEAP_CAPACITY); \
while (0)

#define CHECK_REG(pcval) \
do { \
    ck_assert_ptr_lt(vm.registers.hr, 0); \
    ck_assert_ptr_eq(vm.registers.bp, vm.stack.stack); \
    ck_assert_ptr_eq(vm.registers.sp, vm.stack.stack); \
    ck_assert_ptr_eq(vm.registers.pc, (((char **)vm.rawcode)[1] + sizeof(unsigned long int))); \
} while (0)

#define CHECK_HEAP() \
do {\
    ck_assert_ptr_null(vm.heap.list); \
    ck_assert_int_eq(vm.heap.survive_flag, DEFAULT_SURVIVE_FLAG); \
    ck_assert_int_eq(vm.heap.current_size, 0); \
} while (0) 

#define CHECK_RAWCODE(sample_rawcode) \
do { \
    ck_assert_int_eq(vm.constant_pool.count, __CST_COUNT(rawcode)); \
    ck_assert_int_eq(vm.instructions.length, __INS_LENGTH(sample_rawcode)); \
    ck_assert_ptr_eq(vm.constant_pool.positions, __ARRAY_OF_PTR_TO_CONSTANTS(sample_rawcode)) \
    ck_assert_ptr_eq(vm.instructions.inslist, __ARRAY_OF_INSTRUCTION(sample_rawcode)) \
} while (0)

/* The Index and real data is ommited */
struct sample_rawcode {
    void *idx[2];
    unsigned long int constant_count, instruction_len;
};

void *create_sample_rawcode(unsigned long int constant_count, unsigned long int instruction_len) {
    struct sample_rawcode *ret = (struct sample_rawcode *)malloc(struct sample_rawcode);
    ret->idx[0] = &ret->constant_count;
    ret->idx[1] = &ret->instruction_len;
    ret->constant_count = constant_count;
    ret->instruction_len = instruction_len;

    return (void *)ret;
}

void free_sample_rawcode(struct sample_rawcode *p) {
    free(p);
}


START_TEST(test_load_bytecode)
{
    unsigned long int constant_number = 10, ins_area_length = 100;
    setup_sample_bytecode_file(constant_number, ins_area_length);
    load(SMAPLE_BYTECODE_FILE_NAME);
    CHECK_CONF();
    CHECK_REG(pcval); 
    CHECK_HEAP();
    CHECK_RAWCODE(cst_count, inslen);
    teardown_sample_bytecode_file(); 
}
END_TEST

void setup_sample_bytecode_file(unsigned long int constant_number, unsigned long int ins_area_length) {

    FILE *fp = fopen(SAMPLE_BYTECODE_NAME, "wb+");
    struct sample_bytecode *sample = (struct char *)malloc(sizeof(struct sample_bytecode));

    sample->idx[0] = (void *)&sample->cstcount;
    sample->idx[1] = (void *)&sample->inslen;
    sample->cstcount = constant_number;
    sample->inslen = ins_area_length;

    fwrite((void *)(&sample), sizeof(sample), 1, fp);
}

void teardown_sample_bytecode_file() { 
    remove(SAMPLE_BYTECODE_NAME);    
}

int main() {
    // setup_vm_config();
   // teardown_vm_config();
    return 0;
}
