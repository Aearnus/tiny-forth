#include "types.h"
#include "memory.h"

void compileWord(const char* wordName, ForthToken* definition, size_t definitionLength) {
    ForthDef def;
    def.name = wordName;
    def.definition = definition;
    def.definitionLength = definitionLength;
    dictionary[dictionaryLength++] = def;
}
