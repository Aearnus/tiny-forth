#pragma once

#include "directives.h"

struct ForthWord_S {
    char* name;
    struct ForthWord_S* definition;
};
typedef struct ForthWord_S ForthWord;

enum ForthType_E {
    WORD,
    NUMBER
};
typedef enum ForthType_E ForthType;

struct ForthToken_S {
    char* name;
    ForthType isA;
};
typedef struct ForthToken_S ForthToken;

enum StackError_E {
    STACK_SUCCESS,
    STACK_OVERFLOW_ERROR,
    STACK_UNDERFLOW_ERROR
};
typedef enum StackError_E StackError;
struct StackResult_S {
    StackError e;
    STACK_TYPE result;
};
typedef struct StackResult_S StackResult;
