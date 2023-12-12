#pragma once
#include <wchar.h>
#include <stdbool.h>

typedef struct {
    wchar_t * text;
    int size;
    bool isEnd;
} Sentence ;

typedef struct  {
    Sentence * sentences;
    int size;
} Text ;
