#include <string.h>
#include <stdio.h>

#include "types.h"
#include "memory.h"
#include "compile.h"

//returns if the interpreter must skip ahead steps: ex, there is a definition
int executeWord(int index, ForthToken* tokens, size_t tokenLength) {
    char* word = tokens[index].name;
    //for a definition
    int definitionLength = 0;
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
        printf("\e[38;5;196m%lli\e[0m\n", a.result);
	}
	else if (strcmp(word, "EMIT") == 0) {
        StackResult a = popStack();
        if (a.e != STACK_SUCCESS) {
            goto STACK_UNDERFLOW_ERROR;
        }
        printf("%c\n", a.result % 256);
	}
    //compilation words!
    else if (strcmp(word, ":") == 0) {
        #ifdef DEBUG
            printf("BEGIN DEFINITION\n");
        #endif
        int definitionLength = 1;
        //start the counter to tell the executor to skip to the end of the definition
        while (index + definitionLength < tokenLength) {
            if (strcmp(tokens[index + definitionLength].name, ";") == 0) {
                break;
            }
            definitionLength++;
        }
        //if it broke because we were outside the bounds instead of because we
        //found a semicolon word
        if (index + definitionLength >= tokenLength) {
            goto DEFINITION_UNCLOSED_ERROR;
        } else {
            //make sure it is not a malformed definition
            //AKA: make sure we have a word and a definition
            if (definitionLength < 3) {
                goto DEFINITION_IMPROPER_ERROR;
            }
            #ifdef DEBUG
                printf("SUCCESSFUL DEFINITION OF WORD %s. DEFINITION LENGTH %i.\n", tokens[index + 1].name, definitionLength);
            #endif
            compileWord(tokens[index + 1].name, NULL, definitionLength);
        }
        #ifdef DEBUG
            printf("END DEFINITION\n");
        #endif
    }
    else if (strcmp(word, ";") == 0) {
        //we should never have to parse one of these! they should be skipped
        //by the handler on :
        goto DEFINITION_UNOPENED_ERROR;
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

    DEFINITION_UNCLOSED_ERROR:
    printf("UNCLOSED COLON\n");
    goto WORD_END_HANDLING;

    DEFINITION_UNOPENED_ERROR:
    printf("UNOPENED SEMICOLON\n");
    goto WORD_END_HANDLING;

    DEFINITION_IMPROPER_ERROR:
    printf("IMPROPER (MALFORMED) DEFINITION\n");
    goto WORD_END_HANDLING;

	WORD_SUCCESS:
    #ifdef DEBUG
        printf("RAN %s SUCCESSFULLY\n", word);
        printf("TOP OF STACK: %i %i %i\n", stackLength < 3 ? -1 : stack[stackLength - 3], stackLength < 2 ? -1 : stack[stackLength - 2], stack[stackLength - 1]);
        printf("STACK LENGTH: %i\n", stackLength);
    #endif
    goto WORD_END_HANDLING;

    WORD_END_HANDLING:
    free(word);
    return definitionLength;
}
