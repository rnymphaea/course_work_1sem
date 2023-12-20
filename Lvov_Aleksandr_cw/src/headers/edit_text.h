#pragma once
#include "structs.h"
#include <stdlib.h>
#include <wchar.h>
#include <wctype.h>


wchar_t * toLowerSentence(Sentence sentence);
bool inText(Sentence * sentences, Sentence sentence, int sizeOfText);
bool isUnique(wchar_t ** currentUniqueWords, int countUniqueWords, wchar_t * word);
wchar_t ** getSplittedText(Sentence sentence, int * size, wchar_t * delims, bool lower);
int getCountInSplittedText(wchar_t ** splittedText, int size, wchar_t * word, int currIndex);
int getCountRepeatsInSentence(Sentence sentence);
bool containsLowerFirstLetters(Sentence sentence);
Text deleteSentences(Text text);
int compareByVowels(const void * a, const void * b);
int getCountVowels(wchar_t * word);
bool isVowel(wchar_t symbol);
wchar_t * makeSentence(wchar_t * destination, wchar_t ** splittedSentence, wchar_t ** delims, int sizeSplitted);
Text getSortedText(Text text);
bool isAllMissed(wchar_t * mask);
int compareByLength(const void * a, const void * b);
int compareByLetters(const void * a, const void * b);
int getCountLetters(wchar_t * mask);
bool inWords(wchar_t symbol, wchar_t ** splittedSentence, int sizeSplitted, int index);
wchar_t * getTrueMask(wchar_t ** masks, int sizeTmp);
wchar_t * getMask(Sentence sentence);