#include "memory.h"

STACK_TYPE stack[STACK_MAX_LENGTH];
int stackLength = 0;

pool = malloc(POOL_SIZE)

StackResult popStack() {
    StackResult out;
    if (stackLength <= 0) {
        out.e = STACK_UNDERFLOW_ERROR;
        return out;
    }
    out.e = STACK_SUCCESS;
    out.result = stack[--stackLength];
    return out;
}

StackResult peepStack() {
    StackResult out;
    if (stackLength <= 0) {
        out.e = STACK_UNDERFLOW_ERROR;
        return out;
    }
    out.e = STACK_SUCCESS;
    out.result = stack[stackLength - 1];
    return out;
}

StackResult pushStack(STACK_TYPE in) {
    StackResult out;
    if (stackLength >= STACK_MAX_LENGTH) {
        out.e = STACK_OVERFLOW_ERROR;
    }
    out.e = STACK_SUCCESS;
    stack[stackLength++] = in;
    return out;
}
