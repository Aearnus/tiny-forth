#include <string.h>
#include <stdio.h>

#include "types.h"
#include "memory.h"
#include "compile.h"
#include "run.h"

//returns if the interpreter must skip ahead steps: ex, there is a definition
int executeWord(int index, ForthToken* tokens, size_t tokenLength) {
    char* word = tokens[index].name;
    //for a definition
    int defLength = 0;
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
        printf("%c", a.result % 256);
	}
    //compilation words!
    else if (strcmp(word, ":") == 0) {
        #ifdef DEBUG
            printf("BEGIN DEFINITION\n");
        #endif
        defLength = 1;
        //start the counter to tell the executor to skip to the end of the definition
        //also, begin copying tokens to the definitionArray
        while (index + defLength < tokenLength) {
            if (strcmp(tokens[index + defLength].name, ";") == 0) {
                break;
            }
            defLength++;
        }
        //if it broke because we were outside the bounds instead of because we
        //found a semicolon word
        if (index + defLength >= tokenLength) {
            goto DEFINITION_UNCLOSED_ERROR;
        } else {
            //make sure it is not a malformed definition
            //AKA: make sure we have a word and a definition
            if (defLength < 3) {
                goto DEFINITION_IMPROPER_ERROR;
            }
            #ifdef DEBUG
                printf("SUCCESSFUL DEFINITION OF WORD %s. DEFINITION LENGTH %i.\nDEFINTION:\n    ", tokens[index + 1].name, defLength);
            #endif
            //if EVERYTHING is good, allocate memory and go through and copy tokens
            ForthToken def[defLength - 2];
            for (int defIndex = 2; defIndex < defLength; defIndex++) {
                //justification for this magic number is at the bottom of
                //compile.c. it is also copied here:
                //
                //there is a magic number here:
                //the definition length from the other file actually
                //also contains the name and the ; at the end of the
                //definition. thus, the - 2 removes those two.
                int defOutIndex = defIndex - 2;
                ForthToken currentSrcToken = tokens[index + defIndex];
                ForthToken currentDefToken;
                currentDefToken.isA = currentSrcToken.isA;
                currentDefToken.name = strCpyNew(currentSrcToken.name);
                def[defOutIndex] = currentDefToken;
                #ifdef DEBUG
                    printf("%s ", def[defOutIndex]);
                #endif
            }
            compileWord(strCpyNew(tokens[index + 1].name), def, defLength);
        }
        #ifdef DEBUG
            printf("\nEND DEFINITION\n");
        #endif
    }
    else if (strcmp(word, ";") == 0) {
        //we should never have to parse one of these! they should be skipped
        //by the handler on :
        goto DEFINITION_UNOPENED_ERROR;
    }

    else {
        // dictionary and dictionaryLength are defined in memory.h
        #ifdef DEBUG
            printf("GOING THROUGH DICTIONARY\n");
        #endif
        for (int dictIndex = 0; dictIndex < dictionaryLength; dictIndex++) {
            ForthDef currentDef = dictionary[dictIndex];
            if (strcmp(word, currentDef.name) == 0) {
                #ifdef DEBUG
                    printf("RUNNING CUSTOM WORD %s\nDEFINITION LENGTH %i\nDEFINITION FROM EXECUTEWORD():\n   ", currentDef.name, currentDef.definitionLength);
                    for (int i = 0; i < currentDef.definitionLength; i++) {
                        printf("%s ", currentDef.definition[i]);
                    }
                    printf("\n");
                #endif
                runDefinition(currentDef);
                goto WORD_SUCCESS;
            }
        }
        //if the word couldn't be found in the dictionary
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
        printf("TOP OF STACK: %i %i %i\n", stackLength < 3 ? -1 : stack[stackLength - 3], stackLength < 2 ? -1 : stack[stackLength - 2], stackLength < 1 ? -1 : stack[stackLength - 1]);
        printf("STACK LENGTH: %i\n", stackLength);
    #endif
    goto WORD_END_HANDLING;

    WORD_END_HANDLING:
    #ifdef DEBUG
        if (defLength != 0) {
            printf("SKIPPING AHEAD %i WORDS\n", defLength);
        }
    #endif
    free(word); word = NULL;
    return defLength;
}
