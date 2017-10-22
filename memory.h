#pragma once

#include "types.h"
#include "directives.h"

StackResult popStack();
StackResult peepStack();
StackResult pushStack(STACK_TYPE in);

extern STACK_TYPE stack[];
extern int stackLength;

extern char* pool;

extern ForthWord dictionary[DICTIONARY_LENGTH];
