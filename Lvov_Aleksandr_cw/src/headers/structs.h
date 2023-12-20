#pragma once
#include <wchar.h>
#include <stdbool.h>

typedef struct {
    wchar_t * text; // массив символов
    int size; // размер предложения в символах
    bool isEnd; // является ли предложение концом текста
} Sentence ;

typedef struct  {
    Sentence * sentences; // массив структур Sentence
    int size; // количество предложений
} Text ;
