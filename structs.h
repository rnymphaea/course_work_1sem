#pragma once
#include <wchar.h>
#include <stdbool.h>

struct Sentence {
    wchar_t * text;
    int size;
    bool is_end;
};

struct Text {
    struct Sentence * sentences;
    int size;
};
