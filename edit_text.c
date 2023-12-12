#include "edit_text.h"

wchar_t * toLowerSentence(struct Sentence sentence){
    wchar_t * copied = (wchar_t *)malloc(wcslen(sentence.text) * sizeof(wchar_t) + 1);
    wcscpy(copied, sentence.text);
    for (int i = 0; i < wcslen(sentence.text); i++){
        copied[i] = towlower(copied[i]);
    }
    return copied;
}

bool inText(struct Sentence * sentences, struct Sentence sentence, int sizeOfText){
    if (sizeOfText < 1){
        return false;
    }
    wchar_t * copied = (wchar_t *)malloc(wcslen(sentence.text) * sizeof(wchar_t) + 1);
    copied = toLowerSentence(sentence);
    for (int i = 0; i < sizeOfText; i++){
        wchar_t * curr_sent = (wchar_t *)malloc(wcslen(sentences[i].text) * sizeof(wchar_t) + 1);
        curr_sent = toLowerSentence(sentences[i]);
        if (wcscmp(copied, curr_sent) == 0){
            return true;
            break;
        }
    }
    free(copied);
    return false;
}




