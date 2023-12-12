#pragma once
#include "structs.h"
#include <stdlib.h>
#include <wchar.h>
#include <wctype.h>


wchar_t * toLowerSentence(Sentence sentence);
bool inText(Sentence * sentences, Sentence sentence, int sizeOfText);



