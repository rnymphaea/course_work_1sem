#pragma once
#include <stdio.h>
#include "structs.h"


void printStartInfoCw();
void printInfoFunctions();
int getNumCase();
void controlInput(int numCase);
void printFinalText();
void printNeededWords();
void printCountRepeats();
void printTextWithoutDeleted();
void printSortedText();
void printMask();
bool isBad(wchar_t symbol);
void printMemoryError();
Sentence getString();
Text getText();

