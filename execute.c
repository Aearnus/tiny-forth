#include <string.h>
#include <stdio.h>

#include "types.h"
#include "stack.h"

//returns if the interpreter must skip ahead steps: ex, there is a definition
//TODO: function definition: words : and ;
int executeWord(int index, ForthToken* tokens, int tokenLength) {
    char* word = tokens[index].name;
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
    //compilation words!
    else if (strcmp(word, ":") == 0) {
        int wordsToSkip = 1;
        //start the counter to tell the executor to skip to the end of the definition
        while (index + wordsToSkip < tokenLength) {
            if (strcmp(tokens[index + wordsToSkip].name, ";") == 0) {
                break;
            }
            wordsToSkip++;
        }
        //if it broke because we were outside the bounds instead of because we
        //found a semicolon word
        if (index + wordsToSkip >= tokenLength) {
            goto DEFINITION_UNCLOSED_ERROR;
        } else {
            // TODO: COMPILE
        }
    }
    else if (strcmp(word, ";") == 0) {
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
    printf("RAN %s SUCCESSFULLY\n");
    printf("TOP OF STACK: %i %i %i\n", stackLength < 3 ? -1 : stack[stackLength - 3], stackLength < 2 ? -1 : stack[stackLength - 2], stack[stackLength - 1]);
    printf("STACK LENGTH: %i\n", stackLength);
    goto WORD_END_HANDLING;

    WORD_END_HANDLING:
    return 0;
}
