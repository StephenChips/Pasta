/* This file define functions to assist testing */

/* Function to read bytes from rawcode and convert it to a `struct ins` type valuable */

#ifndef INCLUDED_TESTHELPER_H
#define INCLUDED_TESTHELPER_H

#include "instr.h"

void TestHelper_ReadIns(char *inslist, struct ins *ins);

int is_instr_eq(struct ins expect, struct ins actual);
#endif


