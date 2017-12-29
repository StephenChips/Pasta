#define NO_ERROR 0
#define CONFIG_ERROR 1
#define STACK_OVERFLOW_ERROR 2
#define OUT_OF_MEMOERY_ERROR 3
#define INTERNAL_ERROR 4

#define OK "OK"
#define CANNOT_OPEN_CONFIG_FILE "VMError: Cannot Open Configuration File" 
#define ILLEGAL_CONFIG_FILE_FORMAT "VMError: Illegal Configuration File Format."
#define STACK_CAPACITY_NOT_A_NUMBER "VMError: Stack Capacity Should Be A Number."
#define HEAP_CAPACITY_NOT_A_NUMBER "VMError: Heap Capacity Should Be A Number."
#define STACK_CAPACITY_OUT_OF_RANGE "VMError: Stack Capacity Out Of Range."
#define HEAP_CAPACITY_OUT_OF_RANGE "VMError: Heap Capacity Out Of Range."

#define LOG_ERROR(err_type, msg) \
do { \
    error_logger.err = err_type; \
    error_logger.msg = msg; \
} while (0)

struct {
    vm_error_t err;
    const char *msg;
} error_log; 

