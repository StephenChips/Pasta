union ins_args {
    union {
        int ival;
        double fval;
        char cval;
        void *refval; /* only allow nil value */
    } cst;
    struct {
        unsigned long int addr;
    } gto;
    struct {
        long m;
    } altsp;
    struct {
       unsigned int argnum;
       unsigned long int addr;
   } call;
   struct {
       unsigned int new_argnum, old_argnum;
       unsigned long int addr;
   } tcall;
   struct {
       unsigned int sysfuncid, arg_number;
   } syscall;
   struct {
       unsigned int exn;
   } pushexn;
   struct {
       unsigned int exn;
       unsigned long int addr;
   } popexn;
   struct {
       unsigned int idx;
   } ldc;
};

struct ins_item {
    unsigned char ins_id;
    union ins_args args;
    struct ins_item *next;
};

struct inslist {
    struct ins_item *inslist;
    unsigned int count;
};

struct cstpool_item {
    void *ref;
    size_t size;
    struct cstpool_item *next;
};
        
struct cstpool {
    int count;
    struct cstpool_item *cstlist;
};

struct rawcode {
    struct inslist il;
    struct cstpool cl;
};


struct rawcode *init_rawcode();

void delete_rawcode(struct rawcode * rawcode);

/* If that instruction doesn't require arguments, the union pointer should be NULL */
void rawcode_pushins(unsigned char ins_id, union ins_args *ins_args);

void rawcode_pushconst(void *ref, size_t size);

/* Functions for convenience, they will allocate memory first, then initialize value and call function 'rawcode_pushconst' */
void rawcode_pushchar(char c);

void rawcode_pushstring(char *str, size_t size);

void rawcode_pushdouble(double d);

void rawcode_pushint(int i);

/* generate rawcode */
void *rawcode_generate(struct rawcode *rawcode);
