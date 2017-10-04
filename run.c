#include "run.h"
#include "directives.h"
#include "stack.h"

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

//returns if the interpreter must skip ahead steps: ex, there is a definition
//TODO: function definition: words : and ;
int executeWord(char* word, ForthToken* tokens) {
	//TODO
	//execute a builtin
	if (strcmp(word, "+") == 0) {
        StackResult a = popStack();
        StackResult b = popStack();
        if (a.e != STACK_SUCCESS || b.e != STACK_SUCCESS) {
            goto STACK_UNDERFLOW_ERROR;
        }
        pushStack(a.result + b.result);
	}
	else if (strcmp(word, "-") == 0) {
        StackResult a = popStack();
        StackResult b = popStack();
        if (a.e != STACK_SUCCESS || b.e != STACK_SUCCESS) {
            goto STACK_UNDERFLOW_ERROR;
        }
        pushStack(b.result - a.result);
	}
	else if (strcmp(word, "*") == 0) {
        StackResult a = popStack();
        StackResult b = popStack();
        if (a.e != STACK_SUCCESS || b.e != STACK_SUCCESS) {
            goto STACK_UNDERFLOW_ERROR;
        }
        pushStack(a.result * b.result);
	}
	else if (strcmp(word, "/") == 0) {
        StackResult a = popStack();
        StackResult b = popStack();
        if (a.e != STACK_SUCCESS || b.e != STACK_SUCCESS) {
            goto STACK_UNDERFLOW_ERROR;
        }
        pushStack(b.result / a.result);
	}
    else if (strcmp(word, "/MOD") == 0) {
        StackResult a = popStack();
        StackResult b = popStack();
        if (a.e != STACK_SUCCESS || b.e != STACK_SUCCESS) {
            goto STACK_UNDERFLOW_ERROR;
        }
        pushStack(b.result % a.result);
        pushStack(b.result / a.result);
	}
	else if (strcmp(word, "SWAP") == 0) {
        StackResult a = popStack();
        StackResult b = popStack();
        if (a.e != STACK_SUCCESS || b.e != STACK_SUCCESS) {
            goto STACK_UNDERFLOW_ERROR;
        }
        pushStack(a.result);
        pushStack(b.result);
	}
	else if (strcmp(word, "DUP") == 0) {
        StackResult a = peepStack();
        if (a.e != STACK_SUCCESS) {
            goto STACK_UNDERFLOW_ERROR;
        }
        StackResult b = pushStack(a.result);
        if (b.e != STACK_SUCCESS) {
            goto STACK_OVERFLOW_ERROR;
        }
	}
	else if (strcmp(word, "ROT") == 0) {
        StackResult a = popStack();
        StackResult b = popStack();
        StackResult c = popStack();

        if (a.e != STACK_SUCCESS || b.e != STACK_SUCCESS || c.e != STACK_SUCCESS) {
            goto STACK_UNDERFLOW_ERROR;
        }
        pushStack(a.result);
        pushStack(c.result);
        pushStack(b.result);
	}
	else if (strcmp(word, "DROP") == 0) {
        StackResult a = popStack();
        if (a.e != STACK_SUCCESS) {
            goto STACK_UNDERFLOW_ERROR;
        }
	}
	else if (strcmp(word, ".") == 0) {
        StackResult a = popStack();
        if (a.e != STACK_SUCCESS) {
            goto STACK_UNDERFLOW_ERROR;
        }
        printf("%i\n", a.result);
	}
	else if (strcmp(word, "EMIT") == 0) {
        StackResult a = popStack();
        if (a.e != STACK_SUCCESS) {
            goto STACK_UNDERFLOW_ERROR;
        }
        printf("%i\n", a.result);
	}
    else {
	//TODO: else, go through the dictionary
        goto UNKNOWN_WORD_ERROR;
    }

	//success/error handling:
    //first, make sure these aren't ran accidentally
    //if none of these errors are caught and jumped to,
    //assume the word ran successfully
    goto WORD_SUCCESS;
	STACK_OVERFLOW_ERROR:
    printf("STACK OVERFLOW ERROR\n");
    goto WORD_END_HANDLING;
	STACK_UNDERFLOW_ERROR:
    printf("STACK UNDERFLOW ERROR\n");
    goto WORD_END_HANDLING;
    UNKNOWN_WORD_ERROR:
    printf("UNKNOWN WORD %s\n", word);
    goto WORD_END_HANDLING;
	WORD_SUCCESS:
    printf("RAN %s SUCCESSFULLY\n");
    printf("TOP OF STACK: %i %i %i\n", stackLength < 3 ? -1 : stack[stackLength - 3], stackLength < 2 ? -1 : stack[stackLength - 2], stack[stackLength - 1]);
    printf("STACK LENGTH: %i\n", stackLength);
    goto WORD_END_HANDLING;
    WORD_END_HANDLING:
    return 0;
}
