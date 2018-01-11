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
#define CANNOT_ALLOCATE_MEMORY "VMError: Cannot Allocate Memeory."
#define FILE_DOES_NOT_EXISTS "VMError: File Does Not Exists."

#define EMPTY_STACK "VMError: Empty Stack."
#define DIVIDED_BY_ZERO "VMError: Cannot devivied by zero."
#define ILLEGAL_INDEX "VMError: Illegal index."
#define ILLEGAL_ALTSP_ARGS "VMError: The Size Should Be Non-Negetivie And Lesser Than the Stack's Capacity."
#define ILLEGAL_JUMPING_ADDR "VMError: The Address to Jump is Out of the Intruction Area."
#define ILLEGAL_ARG_COUNT "VMError: The Count of Argument Should be Positive."
#define NO_SUCH_SYSFUNC "VMError: No Such System Function."
#define NO_STACK_FRAME "VMError: No Stack Frame."
#define NO_EXN_HDLR "VMError: No Exception Handler on the Stack."

#define IDX_OUT_OF_RANGE "Exception: Index Out Of Range."
#define NIL_RERERENCE_EXCEPTION "Exception: Ni Reference Exception."
#define LOG_ERROR(err_type, themsg) \
do { \
    error_logger.err = err_type; \
    error_logger.msg = themsg; \
} while (0)

struct {
    int err;
    const char *msg;
} error_logger; 

