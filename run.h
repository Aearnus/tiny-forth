#include "directives.h"

StackResult popStack();
StackResult peepStack();
StackResult pushStack(STACK_TYPE in);
int executeWord(char* word, ForthToken* tokens);
