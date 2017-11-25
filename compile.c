#include <string.h>

#include "types.h"
#include "memory.h"

void compileWord(const char* wordName, ForthToken* definition, int definitionLength) {
    definitionLength -= 2;
    ForthDef def;
    def.name = wordName;
    def.definition = calloc(definitionLength, sizeof(ForthToken));;
    memcpy(def.definition, definition, definitionLength);
    def.definitionLength = definitionLength;
    dictionary[dictionaryLength++] = def;
    //there is a magic number here:
    //the definition length from the other file actually
    //also contains the name and the ; at the end of the
    //definition. thus, the - 2 removes those two.
}
