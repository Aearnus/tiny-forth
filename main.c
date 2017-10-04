#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <string.h>

#include "types.h"
#include "directives.h"
#include "run.h"
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

//establish forth environment
char* pool; //initialized in main
ForthWord dictionary[DICTIONARY_LENGTH];

int main() {
    printf("tiny-forth, by Aearnus. version %s.\nTHIS SOFTWARE COMES WITH ABSOLUTELY NO WARRANTY. IT MAY COMPLETELY DESTROY YOUR COMPUTER.\n", VERSION);
    //initialize environment
    pool = malloc(POOL_SIZE);
    for (;;) {
        //begin read-compile-execute loop
        //read
        char* input;
        input = readline("> ");
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
				tokenIndex += executeWord(tokens[tokenIndex].name, tokens);
			}
		}
	//end repl loop
    }
    //clean up environment
    free(pool);
    free(dictionary);
}
