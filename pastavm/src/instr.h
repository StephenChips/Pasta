#ifndef INCLUDED_INSTR_H
#define INCLUDED_INSTR_H

#define ICONST 0x0
#define FCONST 0x1
#define CCONST 0x2
#define NIL 0x3
#define IADD 0x4
#define FADD 0x5
#define ISUB 0x6
#define FSUB 0x7
#define IMUL 0x8
#define FMUL 0x9
#define IDIV 0xA
#define FDIV 0xB
#define MOD  0xC
#define IEQ  0xD
#define FEQ  0xE
#define REQ  0xF
#define CEQ  0x10
#define ILT  0x11
#define FLT 0x12
#define RLT 0x13
#define CLT 0x14
#define NOT 0x15
#define DUP 0x16
#define LOAD 0x1A
#define STORE 0x1E
#define GETBP  0x22
#define GETSP  0x23
#define ALTSP  0x24
#define JUMP   0x25
#define JPZ    0x26
#define JPNZ   0x27
#define CALL   0x28
#define TCALL  0x29
#define SYSCALL  0x2A
#define RETURN   0x2B
#define STOP     0x2C
#define RAISE    0x2D
#define PUSHEXN  0x2E
#define POPEXN   0x2F
#define IGETDATA 0x30
#define FGETDATA 0x31
#define CGETDATA 0x32
#define RGETDATA 0x33
#define ISETDATA 0x34
#define FSETDATA 0x35
#define CSETDATA 0x36
#define RSETDATA 0x37
#define NEW      0x38
#define I2F      0x39
#define I2C      0x3A
#define F2I      0x3B
#define C2I      0x3C
#define ILDC     0x3D
#define FLDC     0x3E
#define CLDC     0x3F
#define RLDC     0x40

#define INS_ICONST_SIZE (sizeof(char) + sizeof(int))
#define INS_FCONST_SIZE (sizeof(char) + sizeof(double))
#define INS_CCONST_SIZE (sizeof(char) + sizeof(char))
#define INS_IADD_SIZE (sizeof(char))
#define INS_FADD_SIZE (sizeof(char))
#define INS_ISUB_SIZE (sizeof(char))
#define INS_FSUB_SIZE (sizeof(char))
#define INS_IMUL_SIZE (sizeof(char))
#define INS_FMUL_SIZE (sizeof(char))
#define INS_IDIV_SIZE (sizeof(char))
#define INS_FDIV_SIZE (sizeof(char))
#define INS_MOD_SIZE (sizeof(char))
#define INS_IEQ_SIZE (sizeof(char))
#define INS_FEQ_SIZE (sizeof(char))
#define INS_CEQ_SIZE (sizeof(char))
#define INS_REQ_SIZE (sizeof(char))
#define INS_ILT_SIZE (sizeof(char))
#define INS_FLT_SIZE (sizeof(char))
#define INS_CLT_SIZE (sizeof(char))
#define INS_RLT_SIZE (sizeof(char))
#define INS_NOT_SIZE (sizeof(char))
#define INS_DUP_SIZE (sizeof(char))
#define INS_LOAD_SIZE (sizeof(char))
#define INS_STORE_SIZE (sizeof(char))
#define INS_GETSP_SIZE (sizeof(char))
#define INS_GETBP_SIZE (sizeof(char))
#define INS_ALTSP_SIZE (sizeof(char) + sizeof(signed long int))
union ins_args {
    struct {
        int val;
    } iconst;
    struct {
        double val;
    } fconst;
    struct {
        char val;
    } cconst;
    struct {
        unsigned long int addr;
    } jxxx;
    struct {
       signed long int m;
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
       unsigned int sysfuncid;
   } syscall;
   struct {
       unsigned int exn;
   } raise;
   struct {
       unsigned int exn;
       unsigned long int addr;
   } pushexn;
   struct {
       unsigned long int offset;
   } xgetdata;
   struct {
       unsigned long int offset;
   } xsetdata;
   struct {
       unsigned long int refcnt, datasz;
   } new;
   struct {
       unsigned int idx;
   } xldc; /* changed */
};

struct ins {
    unsigned char id;
    union ins_args args;
};

#endif
