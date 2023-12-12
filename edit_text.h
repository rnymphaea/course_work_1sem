#pragma once
#include "structs.h"
#include <stdlib.h>
#include <wchar.h>
#include <wctype.h>


wchar_t * toLowerSentence(struct Sentence sentence);
bool inText(struct Sentence * sentences, struct Sentence sentence, int sizeOfText);



