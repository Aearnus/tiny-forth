#include "types.h"
#include "memory.h"

void compileWord(const char* wordName, ForthToken* definition, int definitionLength) {
    /*
    ForthDef def;
    def.name = wordName;
    def.definition = definition;
    //there is a magic number here:
    //the definition length from the other file actually
    //also contains the name and the ; at the end of the
    //definition. thus, the - 2 removes those two.
    def.definitionLength = definitionLength - 2;
    dictionary[dictionaryLength++] = def;
    */
    dictionary[dictionaryLength].name = wordName;
    dictionary[dictionaryLength].definition = definition;
    dictionary[dictionaryLength].definitionLength = definitionLength - 2;
    dictionaryLength++;
}
