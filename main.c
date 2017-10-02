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
char* pool; //initialized in main
ForthWord dictionary[DICTIONARY_LENGTH]; //initialized in main

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
void executeWord(char* word) {
	//TODO
	//execute a builtin
	if (strcmp(word, "+")) {
        StackResult a = popStack();
        StackResult b = popStack();
        if (a.e != STACK_SUCCESS || b.e != STACK_SUCCESS) {
            goto STACK_UNDERFLOW_ERROR;
        }
        pushStack(a.result + b.result);
	}
	else if (strcmp(word, "-")) {
        StackResult a = popStack();
        StackResult b = popStack();
        if (a.e != STACK_SUCCESS || b.e != STACK_SUCCESS) {
            goto STACK_UNDERFLOW_ERROR;
        }
        pushStack(b.result - a.result);
	}
	else if (strcmp(word, "*")) {
        StackResult a = popStack();
        StackResult b = popStack();
        if (a.e != STACK_SUCCESS || b.e != STACK_SUCCESS) {
            goto STACK_UNDERFLOW_ERROR;
        }
        pushStack(a.result * b.result);
	}
	else if (strcmp(word, "/")) {
        StackResult a = popStack();
        StackResult b = popStack();
        if (a.e != STACK_SUCCESS || b.e != STACK_SUCCESS) {
            goto STACK_UNDERFLOW_ERROR;
        }
        pushStack(b.result / a.result);
	}
    else if (strcmp(word, "DIVMOD")) {
        StackResult a = popStack();
        StackResult b = popStack();
        if (a.e != STACK_SUCCESS || b.e != STACK_SUCCESS) {
            goto STACK_UNDERFLOW_ERROR;
        }
        pushStack(b.result % a.result);
        pushStack(b.result / a.result);
	}
	else if (strcmp(word, "SWAP")) {
        StackResult a = popStack();
        StackResult b = popStack();
        if (a.e != STACK_SUCCESS || b.e != STACK_SUCCESS) {
            goto STACK_UNDERFLOW_ERROR;
        }
        pushStack(a.result);
        pushStack(b.result);
	}
	else if (strcmp(word, "DUP")) {
        StackResult a = peepStack();
        if (a.e != STACK_SUCCESS) {
            goto STACK_UNDERFLOW_ERROR;
        }
        StackResult b = pushStack(a.result);
        if (b.e != STACK_SUCCESS) {
            goto STACK_OVERFLOW_ERROR;
        }
	}
	else if (strcmp(word, "ROT")) {

	}
	else if (strcmp(word, "DROP")) {
        StackResult a = popStack();
        if (a.e != STACK_SUCCESS) {
            goto STACK_UNDERFLOW_ERROR;
        }
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
    printf("TOP OF STACK: %i %i %i\n", stack[0], stack[1], stack[2]);
    goto WORD_END_HANDLING;
    WORD_END_HANDLING:
    ;
}


int main() {
    printf("tiny-forth, by Aearnus. version %s.\nTHIS SOFTWARE COMES WITH ABSOLUTELY NO WARRANTY. IT MAY COMPLETELY DESTROY YOUR COMPUTER.\n", VERSION);
    //initialize environment
    pool = malloc(POOL_SIZE);
    //dictionary = calloc(DICTIONARY_LENGTH, sizeof(ForthWord));
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
    //clean up environment
    free(pool);
    free(dictionary);
}
