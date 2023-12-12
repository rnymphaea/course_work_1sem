#pragma once
#include "structs.h"
#include <stdlib.h>
#include <wchar.h>
#include <wctype.h>


wchar_t * toLowerSentence(Sentence sentence);
bool inText(Sentence * sentences, Sentence sentence, int sizeOfText);
bool isUnique(wchar_t ** currentUniqueWords, int countUniqueWords, wchar_t * word);
wchar_t ** getSplittedText(Sentence sentence, int * size);
int getCountInSplittedText(wchar_t ** splittedText, int size, wchar_t * word, int currIndex);
int getCountRepeatsInSentence(Sentence sentence);


