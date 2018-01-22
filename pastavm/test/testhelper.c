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
    case CALL: case TCALL:
        ins.args.call.argnum = *(unsigned int *)pos;  
        pos += sizeof(unsigned int);
        ins.args.call.addr = *(unsigned long int *)pos;
        break;
    case RAISE:
        ins.args.raise.exn = *(unsigned int *)pos;
        break;
    case PUSHEXN:
        ins.args.pushexn.exn = *(unsigned int *)pos;
        pos += sizeof(unsigned int);
        ins.args.pushexn.addr = *(unsigned long int *)pos;
        break;
    case NEW:
        ins.args.new.count = *(unsigned long int *)pos;
        break;
    case LDC:
        ins.args.ldc.idx = *(unsigned int *)pos;
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

    case CALL: case TCALL:
        return expect.args.call.argnum == actual.args.call.argnum &&
               expect.args.call.addr == actual.args.call.addr;

    case RAISE:
        return expect.args.raise.exn == actual.args.raise.exn;

    case PUSHEXN:
        return expect.args.pushexn.exn == actual.args.pushexn.exn &&
               expect.args.pushexn.addr == actual.args.pushexn.addr;

    case NEW:
        return expect.args.new.count == actual.args.new.count;

    case LDC:
        return expect.args.ldc.idx == actual.args.ldc.idx;
    default:
        return 1;
    }
}
