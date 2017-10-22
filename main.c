#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <string.h>

#include "types.h"
#include "directives.h"
#include "run.h"
#include "memory.h"

//establish forth environment
ForthWord dictionary[DICTIONARY_LENGTH];

int main() {
    printf("tiny-forth, by Aearnus. version %s.\nTHIS SOFTWARE COMES WITH ABSOLUTELY NO WARRANTY. IT MAY COMPLETELY DESTROY YOUR COMPUTER. %i+%i BYTES FREE.\n", VERSION, POOL_SIZE, STACK_MAX_LENGTH * sizeof(STACK_TYPE));
    //initialize environment
    for (;;) {
        //begin read-compile-execute loop
        //read
        char* input;
        input = readline("> ");
        //parse/execute
        runLine(input);
        free(input);
	//end repl loop
    }
    //clean up environment
    free(pool);
    free(dictionary);
}
