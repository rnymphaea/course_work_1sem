#include <wchar.h>
#include <wctype.h>
#include <stdlib.h>
#include <locale.h>
#include <stdbool.h>
#include "io_functions.h"
#include "edit_text.h"

#define BUF_SIZE 30
#define END_OF_STRING L'\0'

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
        case 4:
            printCountRepeats();
            break;
        case 5:
            printInfoFunctions();
            break;
        default:
            wprintf(L"Error: incorrect number of case.");
            break;
    }
}

void printCountRepeats(){
    Text text = getText();
    for (int i = 0; i < text.size; i++){
        wprintf(L"Количество одинаковых слов: %d\n", getCountRepeatsInSentence(text.sentences[i]));
    }
}

void printFinalText(){
    Text text = getText();
    for (int i = 0; i < text.size; i++){
        wprintf(L"%ls\n", text.sentences[i].text);
        free(text.sentences[i].text);
    }
    free(text.sentences);
}

void printInfoFunctions(){
    wprintf(L"%s\n%s\n%s\n%s\n", "1) Вывод строки-образца для каждого предложения.",
    "2) Удаление из текста предложений, в которых нет заглавных букв в начале слова.",
    "3) Сортировка слов в предложении по количеству гласных букв.",
    "4) Вывод количества одинаковых слов для каждого предложения.");
}


Sentence getString(){
    Sentence sentence;
    wchar_t * text = (wchar_t *)malloc(BUF_SIZE * sizeof(wchar_t));
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
        if (countNewLines == 2){
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
                    text = (wchar_t *)realloc(text, curr_buf * sizeof(wchar_t));
                }
            }
        }
    } while (chr != L'.');
    text[size] = END_OF_STRING;
    sentence.text = text;
    sentence.size = size;
    return sentence;
}

Text getText(){
    Text text;
    Sentence * sentences = malloc(BUF_SIZE * sizeof(Sentence));
    Sentence curr_sent;
    int curr_buf = BUF_SIZE;
    int size = 0;
    bool check = false;
    do {
        curr_sent = getString();
        check = inText(sentences, curr_sent, size);
        if (check){
            continue;
        }
        else{
            if (curr_sent.size == 0){
                continue;
            }
            sentences[size] = curr_sent;
            size++;
            if (size == curr_buf - 1){
                curr_buf += BUF_SIZE;
                sentences = (Sentence *)realloc(sentences, curr_buf * sizeof(Sentence));
            }
        }
    } while (curr_sent.isEnd != true);
    text.sentences = sentences;
    text.size = size;
    return text;
}

