#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "run.h"
#include "directives.h"
#include "stack.h"

ForthType whichType(char* name) {
    //char* numberChars = "-0123456789.";
    char* numberChars = "-0123456789";
    int i;
    for (i = 0; i < strlen(name); i++) {
        //if a non-digit appears in the name
        if (!strchr(numberChars, name[i])) {
            return WORD;
        }
    }
    return NUMBER;
}
void runLine(char* input) {
    //parse
    ForthToken tokens[MAX_INPUT_LENGTH];
    int tokenLength = 0;
    char* currentToken;
    currentToken = strtok(input, " ");
    while (currentToken) {
        ForthToken token;
        token.name = malloc(strlen(currentToken) + 1);
        strcpy(token.name, currentToken);
        token.isA = whichType(token.name);
        tokens[tokenLength++] = token;
        currentToken = strtok(NULL, " ");
    }
    //print out parsed tokens for debugging
    #ifdef DEBUG
        int tokenIndex;
        for (tokenIndex = 0; tokenIndex < tokenLength; tokenIndex++) {
            printf("TOKEN: %s\n", tokens[tokenIndex].name);
            printf("       %s\n", (tokens[tokenIndex].isA == WORD ? "WORD" : "NUMBER"));
        }
    #endif
    //execute
    for (int tokenIndex = 0; tokenIndex < tokenLength; tokenIndex++) {
        //handle numbers first
        if (tokens[tokenIndex].isA == NUMBER) {
            STACK_TYPE number;
            pushStack(atoll(tokens[tokenIndex].name));
        } else if (tokens[tokenIndex].isA == WORD) {
            tokenIndex += executeWord(tokenIndex, tokens);
        }
    }
    //free token names
    for (int tokenIndex = 0; tokenIndex < tokenLength; tokenIndex++) {
        free(tokens[tokenIndex].name);
    }
}
//returns if the interpreter must skip ahead steps: ex, there is a definition
//TODO: function definition: words : and ;
int executeWord(int index, ForthToken* tokens) {
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
