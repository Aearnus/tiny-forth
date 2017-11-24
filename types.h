#pragma once
#include "directives.h"

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

struct ForthDef_S {
    const char* name;
    struct ForthToken_S* definition;
    int definitionLength;
};
typedef struct ForthDef_S ForthDef;

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

#define strCpyNew(str) strcpy(malloc(strlen(str)), str)
