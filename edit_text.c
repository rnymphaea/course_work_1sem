#include "edit_text.h"

wchar_t * toLowerSentence(Sentence sentence){
    wchar_t * copied = (wchar_t *)malloc(wcslen(sentence.text) * sizeof(wchar_t) + 1);
    wcscpy(copied, sentence.text);
    for (int i = 0; i < wcslen(sentence.text); i++){
        copied[i] = towlower(copied[i]);
    }
    return copied;
}

bool inText(Sentence * sentences, Sentence sentence, int sizeOfText){
    if (sizeOfText < 1){
        return false;
    }
    // wchar_t * copied = (wchar_t *)malloc(wcslen(sentence.text) * sizeof(wchar_t) + 1);
    wchar_t * copied = toLowerSentence(sentence);
    for (int i = 0; i < sizeOfText; i++){
    //    wchar_t * curr_sent = (wchar_t *)malloc(wcslen(sentences[i].text) * sizeof(wchar_t) + 1);
        wchar_t * curr_sent = toLowerSentence(sentences[i]);
        if (wcscmp(copied, curr_sent) == 0){
            return true;
            break;
        }
    }
    free(copied);
    return false;
}

bool isUnique(wchar_t ** currentUniqueWords, int countUniqueWords,wchar_t * word){
    if (countUniqueWords == 0){
        return true;
    }
    for (int i = 0; i < countUniqueWords; i++){
        if (wcscmp(word, currentUniqueWords[i]) == 0){
            return false;
            break;
        }
    }
    return true;
}

wchar_t ** getSplittedText(Sentence sentence, int * size){
    wchar_t ** result = (wchar_t **)malloc(sentence.size * sizeof(wchar_t *) + 1);
    wchar_t * copied = toLowerSentence(sentence);
    wchar_t * pt;
    wchar_t * currentWord;
    wchar_t * delims = L" ,.-\n";
    currentWord = wcstok(copied, delims, &pt);
    while (currentWord != NULL){
        result[(*size)++] = currentWord;
        currentWord = wcstok(NULL, delims, &pt);
    }
    return result;
}

int getCountInSplittedText(wchar_t ** splittedText, int size, wchar_t * word, int currIndex){
    int count = 0;
    for (int i = 0; i < size; i++){
        if (i == currIndex){
            continue;
        }
        else if (wcscmp(splittedText[i], word) == 0){
            count++;
        }
    }
    return count;
}

int getCountRepeatsInSentence(Sentence sentence){
    wchar_t ** uniqueWords = (wchar_t **)malloc(sentence.size * sizeof(wchar_t *) + 1);
    int countUniqueWords = 0;
    int countRepeats = 0;
    int sizeSplittedText = 0;
    wchar_t * copied = toLowerSentence(sentence);
    wchar_t ** splittedText = getSplittedText(sentence, &sizeSplittedText);
    for (int i = 0; i < sizeSplittedText; i++){
        if (isUnique(uniqueWords, countUniqueWords, splittedText[i]) == true){
            int result = getCountInSplittedText(splittedText, sizeSplittedText, splittedText[i], i);
            if (result > 0){
                countRepeats += result + 1;
            }
            uniqueWords[countUniqueWords++] = splittedText[i];
        }
    }

    return countRepeats;

}



