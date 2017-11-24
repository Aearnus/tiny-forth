#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "types.h"
#include "run.h"
#include "directives.h"
#include "memory.h"
#include "execute.h"

ForthType whichType(const char* name) {
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
            tokenIndex += executeWord(tokenIndex, tokens, tokenLength);
        }
    }
    //free token names
    for (int tokenIndex = 0; tokenIndex < tokenLength; tokenIndex++) {
        //TODO: FIX THIS. WHY CAN'T I FREE THE TOKEN NAMES? THIS IS A MEMORY LEAK
        //free(tokens[tokenIndex].name);
        //ACTUALLY, THESE WORDS ARE PROBABLY FREED AT THE END OF EXECUTE.C'S EXECUTEWORD()
    }
}

//skip the parse
void runDefinition(ForthToken* tokens) {
    //execute
    for (int tokenIndex = 0; tokenIndex < tokenLength; tokenIndex++) {
        //handle numbers first
        if (tokens[tokenIndex].isA == NUMBER) {
            STACK_TYPE number;
            pushStack(atoll(tokens[tokenIndex].name));
        } else if (tokens[tokenIndex].isA == WORD) {
            tokenIndex += executeWord(tokenIndex, tokens, tokenLength);
        }
    }
}
