#define DICTIONARY_LENGTH 1024
#define MAX_WORD_SIZE 128
#define MAX_WORD_LENGTH 1024
#define MAX_INPUT_LENGTH 1024
#define STACK_MAX_LENGTH 16384
#define STACK_TYPE int64_t
#define POOL_SIZE 1048576
#define VERSION "v0.0.1a"

#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <string.h>

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

struct ForthToken_S {
    char* name;
    ForthType isA;
};
typedef struct ForthToken_S ForthToken;

//establish forth environment
STACK_TYPE stack[STACK_MAX_LENGTH];
int stackLength = 0;
char* pool = malloc(POOL_SIZE);
ForthWord dictionary[DICTIONARY_LENGTH];

STACK_TYPE popStack() {
    if (stackLength <= 0) {
        goto STACK_UNDERFLOW_ERROR;
    }
    return stack[--stackLength];
}
STACK_TYPE peepStack() {
    if (stackLength <= 0) {
        goto STACK_UNDERFLOW_ERROR;
    }
    return stack[stackLength - 1];
}
void pushStack(STACK_TYPE in) {
    if (stackLength >= STACK_MAX_LENGTH) {
        goto STACK_OVERFLOW_ERROR;
    }
    stack[stackLength++] = in;
}
void executeWord(char* word) {
	//TODO
	//execute a builtin
	if (strcmp(word, "+")) {
        int a = popStack();
        int b = popStack();
        pushStack(a + b);
	}
	else if (strcmp(word, "-")) {
        int a = popStack();
        int b = popStack();
        pushStack(b - a);
	}
	else if (strcmp(word, "*")) {
        int a = popStack();
        int b = popStack();
        pushStack(a * b);
	}
	else if (strcmp(word, "/")) {
        int a = popStack();
        int b = popStack();
        pushStack(b / a);
	}
    else if (strcmp(word, "DIVMOD")) {
        int a = popStack();
        int b = popStack();
        pushStack(b % a);
        pushStack(b / a);
	}
	else if (strcmp(word, "SWAP")) {
        int a = popStack();
        int b = popStack();
        pushStack(a);
        pushStack(b);
	}
	else if (strcmp(word, "DUP")) {
        int a = peepStack();
        pushStack(a);
	}
	else if (strcmp(word, "ROT")) {

	}
	else if (strcmp(word, "DROP")) {
        popStack();
	}
	else if (strcmp(word, ".")) {

	}
	else if (strcmp(word, "EMIT")) {

	}
    else {
	//TODO: else, go through the dictionary
        goto UNKNOWN_WORD_ERROR;
    }

	//success/error handling:
	STACK_OVERFLOW_ERROR:
    ;
	STACK_UNDERFLOW_ERROR:
    ;
    UNKNOWN_WORD_ERROR:
    ;
	WORD_SUCCESS:
    ;
}


int main() {
    printf("tiny-forth, by Aearnus. version %s.\nTHIS SOFTWARE COMES WITH ABSOLUTELY NO WARRANTY. IT MAY COMPLETELY DESTROY YOUR COMPUTER.\n", VERSION);
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
				executeWord(tokens[tokenIndex].name);
			}
		}
	//end repl loop
    }
}
