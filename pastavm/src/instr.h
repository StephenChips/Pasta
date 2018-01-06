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
#define IDUP 0x16
#define FDUP 0x17
#define RDUP 0x18
#define CDUP 0x19
#define ILOAD 0x1A
#define FLOAD 0x1B
#define RLOAD 0x1C
#define CLOAD 0x1D
#define ISTORE 0x1E
#define FSTORE 0x1F
#define RSTORE 0x20
#define CSTORE 0x21
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
#define ISETDATA 0x33
#define FSETDATA 0x34
#define CSETDATA 0x35
#define GETREF   0x36
#define SETREF   0x37
#define NEW      0x38
#define I2F      0x39
#define I2C      0x3A
#define F2I      0x3B
#define C2I      0x3C
#define LDC      0x3D

union ins_args {
    int iconst;
    double fconst;
    char cconst;

    struct {
        unsigned long int addr;
    } jump, jpz, jpnz;
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
       unsigned int offset;
   } getdata;
   struct {
       unsigned int offset;
   } setdata;
   struct {
       unsigned int idx;
   } ldc;
};

struct ins {
    unsigned char id;
    union ins_args args;
};

#endif
