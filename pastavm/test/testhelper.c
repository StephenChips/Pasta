#include "testhelper.h"
#include "instr.h"

void TestHelper_ReadIns(char *inslist, struct ins *instr) {

    struct ins ins;
    char *pos = inslist;

    ins.id = *inslist;
    pos++;
     
    switch(ins.id) { 
    case ICONST: 
        ins.args.iconst.val = *(int *)pos;
        break;
    case FCONST:
        ins.args.fconst.val = *(double *)pos;
        break;
    case CCONST:
        ins.args.cconst.val = *(char *)pos;
        break;
    case JUMP: case JPZ: case JPNZ:
        ins.args.jxxx.addr = *(unsigned long int *)pos;
        break;
    case ALTSP:
        ins.args.altsp.m = *(signed long int *)pos;
        break;
    case CALL:
        ins.args.call.argnum = *(unsigned int *)pos;  
        pos += sizeof(unsigned int);
        ins.args.call.addr = *(unsigned long int *)pos;
        break;
    case TCALL:
        ins.args.tcall.new_argnum = *(unsigned int *)pos; 
        pos += sizeof(unsigned int);
        ins.args.tcall.old_argnum = *(unsigned int *)pos; 
        pos += sizeof(unsigned int);
        ins.args.call.addr = *(unsigned long int *)pos;
        break;
    case SYSCALL:
        ins.args.syscall.sysfuncid = *(unsigned int *)pos;
        break;
    case RAISE:
        ins.args.raise.exn = *(unsigned int *)pos;
        break;
    case PUSHEXN:
        ins.args.pushexn.exn = *(unsigned int *)pos;
        pos += sizeof(unsigned int);
        ins.args.pushexn.addr = *(unsigned long int *)pos;
        break;
    case IGETDATA: case FGETDATA: case CGETDATA:
        ins.args.xgetdata.offset = *(unsigned long int *)pos;
        break;
    case ISETDATA: case FSETDATA: case CSETDATA:
        ins.args.xsetdata.offset = *(unsigned long int *)pos;
        break;
    case NEW:
        ins.args.new.refcnt = *(unsigned long int *)pos;
        pos += sizeof(unsigned long int);
        ins.args.new.datasz = *(unsigned long int *)pos;
        break;
    case ILDC: case FLDC: case RLDC: case CLDC:
        ins.args.xldc.idx = *(unsigned int *)pos;
        break;
    }

    *instr = ins;
}

int is_instr_eq(struct ins expect, struct ins actual) {

    if (expect.id != actual.id) {
        return 0;
    }

    switch(expect.id) { 
    case ICONST: 
        return expect.args.iconst.val == actual.args.iconst.val;

    case FCONST:
        return expect.args.fconst.val == actual.args.fconst.val;

    case CCONST:
        return expect.args.cconst.val == actual.args.fconst.val;

    case JUMP:
        return expect.args.jxxx.addr == actual.args.jxxx.addr;

    case JPZ:
        return expect.args.jxxx.addr == actual.args.jxxx.addr;

    case JPNZ:
       return expect.args.jxxx.addr == actual.args.jxxx.addr;

    case ALTSP:
        return expect.args.altsp.m == actual.args.altsp.m;

    case CALL:
        return expect.args.call.argnum == actual.args.call.argnum &&
               expect.args.call.addr == actual.args.call.addr;

    case TCALL:
        return expect.args.tcall.new_argnum == actual.args.tcall.new_argnum && 
               expect.args.tcall.old_argnum == actual.args.tcall.old_argnum &&
               expect.args.call.addr == actual.args.tcall.addr;

    case SYSCALL:
        return expect.args.syscall.sysfuncid == actual.args.syscall.sysfuncid;

    case RAISE:
        return expect.args.raise.exn == actual.args.raise.exn;

    case PUSHEXN:
        return expect.args.pushexn.exn == actual.args.pushexn.exn &&
               expect.args.pushexn.addr == actual.args.pushexn.addr;

    case IGETDATA: case FGETDATA: case CGETDATA:
        return expect.args.xgetdata.offset == actual.args.xgetdata.offset;

    case ISETDATA: case FSETDATA: case CSETDATA:
        return expect.args.xsetdata.offset == actual.args.xsetdata.offset;

    case NEW:
        return expect.args.new.refcnt == actual.args.new.refcnt &&
               expect.args.new.datasz == actual.args.new.datasz;

    case ILDC: case FLDC: case RLDC: case CLDC:
        return expect.args.xldc.idx == actual.args.xldc.idx;
    default:
        return 1;
    }
}
