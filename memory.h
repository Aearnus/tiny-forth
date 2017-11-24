#pragma once

#include "types.h"
#include "directives.h"

StackResult popStack();
StackResult peepStack();
StackResult pushStack(STACK_TYPE in);

void initMemory();
void freeMemory();

extern STACK_TYPE stack[];
extern int stackLength;
extern char* pool;

extern ForthDef dictionary[];
extern int dictionaryLength;
