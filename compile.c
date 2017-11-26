#include <string.h>
#include <stdio.h>

#include "types.h"
#include "memory.h"

void compileWord(const char* wordName, ForthToken* definition, int definitionLength) {
    definitionLength -= 2;
    #ifdef DEBUG
        printf("\nSAVING DEFINITION:\n    ");
        for (int i = 0; i < definitionLength; i++) {
            printf("%s ", definition[i]);
        }
    #endif
    ForthDef def;
    def.name = wordName;
    def.definition = calloc(definitionLength, sizeof(struct ForthToken_S));
    memcpy(def.definition, definition, definitionLength * sizeof(struct ForthToken_S));
    def.definitionLength = definitionLength;
    dictionary[dictionaryLength++] = def;
    //there is a magic number here:
    //the definition length from the other file actually
    //also contains the name and the ; at the end of the
    //definition. thus, the - 2 removes those two.
    #ifdef DEBUG
        printf("\nSAVED DEFINITION:\n    ");
        for (int i = 0; i < dictionary[dictionaryLength - 1].definitionLength; i++) {
            printf("%s ", dictionary[dictionaryLength - 1].definition[i]);
        }
    #endif
}
