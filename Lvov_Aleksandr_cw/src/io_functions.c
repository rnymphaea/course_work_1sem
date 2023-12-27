#include <wchar.h>
#include <wctype.h>
#include <stdlib.h>
#include <locale.h>
#include <stdbool.h>

#include "headers/io_functions.h"
#include "headers/edit_text.h"
#include "headers/defines.h"


void printStartInfoCw(){
    wprintf(L"Course work for option 5.3, created by Aleksandr Lvov.\n");
}

int getNumCase(){
    int numCase;
    wscanf(L"%d", &numCase);
    char c = getwchar();
    return numCase;
}

void controlInput(int numCase){
    switch (numCase){
        case 0:
            printFinalText();
            break;
        case 1:
            printMask();
            break;
        case 2:
            printTextWithoutDeleted();
            break;
        case 3:
            printSortedText();
            break;
        case 4:
            printCountRepeats();
            break;
        case 5:
            printInfoFunctions();
            break;
        case 9:
            printNeededWords();
            break;
        default:
            wprintf(L"Error: incorrect number of case.\n");
            break;
    }
}

void printNeededWords(){
    Text text = getText();
    wchar_t * startWord = getStartWord(text);
    printTrueWords(text, startWord);
}

void printCountRepeats(){
    Text text = getText();
    for (int i = 0; i < text.size; i++){
        wprintf(L"Количество одинаковых слов: %d\n", getCountRepeatsInSentence(text.sentences[i]));
        free(text.sentences[i].text);
    }
    free(text.sentences);
}

void printFinalText(){
    Text text = getText();
    for (int i = 0; i < text.size; i++){
        wprintf(L"%ls\n", text.sentences[i].text);
        free(text.sentences[i].text);
    }
    free(text.sentences);
}

void printTextWithoutDeleted(){
    Text text = getText();
    text = deleteSentences(text);
    for (int i = 0; i < text.size; i++){
        wprintf(L"%ls\n", text.sentences[i].text);
        free(text.sentences[i].text);
    }
}

void printSortedText(){
    Text text = getText();
    text = getSortedText(text);
    for (int i = 0; i < text.size; i++){
        wprintf(L"%ls\n", text.sentences[i].text);
    }
}

void printMask(){
    Text text = getText();
    for (int i = 0; i < text.size; i++){
        wchar_t * mask = getMask(text.sentences[i]);
        wprintf(L"%ls\n", mask);
    }
}

void printInfoFunctions(){
    wprintf(L"%ls\n%ls\n%ls\n%ls\n%ls\n", L"0) Первичная обработка текста.",
    L"1) Вывод строки-образца для каждого предложения.",
    L"2) Удаление из текста предложений, в которых нет заглавных букв в начале слова.",
    L"3) Сортировка слов в предложении по количеству гласных букв.",
    L"4) Вывод количества одинаковых слов для каждого предложения.");
}

void printMemoryError(){
    wprintf(L"Error: cannot allocate memory\n");
}

bool isBad(wchar_t symbol){
    const wchar_t * BAD = L"!@#$%^&*№;:?*[{}()]<+->=\"'_~`/|\\";
    for (int i = 0; i < wcslen(BAD); i++){
        if (symbol == BAD[i]){
            return true;
            break;
        }
    }
    return false;
}

Sentence getString(){
    Sentence sentence;
    wchar_t * text = (wchar_t *)calloc(BUF_SIZE, sizeof(wchar_t));
    if (text == NULL){
        printMemoryError();
    }
    int size = 0;
    int curr_buf = BUF_SIZE;
    sentence.isEnd = false;
    int countNewLines = 0;
    wchar_t chr;
    
    do {
        chr = getwchar();
        if (chr == L'\n'){
            countNewLines++;
        }
        if (isBad(chr)){
            continue;
        }
        if (countNewLines == 2){
            text[size++] = END_OF_STRING;
            sentence.text = text;
            sentence.size = size;
            sentence.isEnd = true;
            return sentence;
            break;
        }
        else{
            if (iswspace(chr) && size == 0){
                continue;
            }
            else{
                text[size++] = chr;
                countNewLines = 0;
                if (size == curr_buf - 1){
                    curr_buf += BUF_SIZE;
                    wchar_t * tmp = (wchar_t *)realloc(text, curr_buf * sizeof(wchar_t));
                    if (tmp != NULL){
                        text = tmp;
                    }
                    else {
                        printMemoryError();
                    }
                }
            }
        }
    } while (chr != L'.');
    text[size++] = END_OF_STRING;
    sentence.text = text;
    sentence.size = size;
    return sentence;
}

Text getText(){
    Text text;
    Sentence * sentences = (Sentence *)calloc(BUF_SIZE, sizeof(Sentence));
    if (sentences == NULL){
        printMemoryError();
    }
    Sentence currSentence;
    int currBuf = BUF_SIZE;
    int size = 0;
    bool check = false;
    do {
        currSentence = getString();
        check = inText(sentences, currSentence, size);
        if (check){
            continue;
        }
        else{
            if (currSentence.size == 0){
                continue;
            }
            sentences[size++] = currSentence;
            if (size == currBuf - 1){
                currBuf += BUF_SIZE;
                Sentence * tmp = (Sentence *)realloc(sentences, currBuf * sizeof(Sentence));
                if (tmp != NULL){
                    sentences = tmp;
                }
                else {
                    printMemoryError();
                }
            }
        }
    } while (currSentence.isEnd != true);
    text.sentences = sentences;
    text.size = size - 1;
    return text;
}


