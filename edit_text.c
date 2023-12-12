#include "edit_text.h"
#define DEFAULT_DELIMETERS L" ,.-\n"
wchar_t * toLowerSentence(Sentence sentence){
    wchar_t * lowerSentence = (wchar_t *)malloc(wcslen(sentence.text) * sizeof(wchar_t) + 1);
    wcscpy(lowerSentence, sentence.text);
    for (int i = 0; i < wcslen(sentence.text); i++){
        lowerSentence[i] = towlower(lowerSentence[i]);
    }
    return lowerSentence;
}

bool inText(Sentence * sentences, Sentence sentence, int sizeOfText){
    if (sizeOfText < 1){
        return false;
    }
    wchar_t * lowerSentence = toLowerSentence(sentence);
    for (int i = 0; i < sizeOfText; i++){
        wchar_t * curr_sent = toLowerSentence(sentences[i]);
        if (wcscmp(lowerSentence, curr_sent) == 0){
            free(lowerSentence);
            return true;
            break;
        }
    }
    free(lowerSentence);
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

wchar_t ** getSplittedText(Sentence sentence, int * size, wchar_t * delims, bool lower){
    wchar_t ** result = (wchar_t **)malloc(sentence.size * sizeof(wchar_t *) + 1);
    wchar_t * currSentence;
    if (lower == true){
        currSentence = toLowerSentence(sentence);
    }
    else {
        wchar_t * copyString = (wchar_t *)malloc(sentence.size * sizeof(wchar_t *) + 1);
        wcscpy(copyString, sentence.text);
        currSentence = copyString;
    }
    wchar_t * valueToken; // state of the current tokenization sequence.
    wchar_t * currentWord;
    currentWord = wcstok(currSentence, delims, &valueToken);
    while (currentWord != NULL){
        result[(*size)++] = currentWord;
        currentWord = wcstok(NULL, delims, &valueToken);
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
    wchar_t ** splittedText = getSplittedText(sentence, &sizeSplittedText, DEFAULT_DELIMETERS, true);
    for (int i = 0; i < sizeSplittedText; i++){
        if (isUnique(uniqueWords, countUniqueWords, splittedText[i]) == true){
            int result = getCountInSplittedText(splittedText, sizeSplittedText, splittedText[i], i);
            if (result > 0){
                countRepeats += result + 1;
            }
            uniqueWords[countUniqueWords++] = splittedText[i];
        }

    }

    free(splittedText);
    free(uniqueWords);

    return countRepeats;

}

bool containsLowerFirstLetters(Sentence sentence){
    int sizeSplitted = 0;
    wchar_t ** splittedSentence = getSplittedText(sentence, &sizeSplitted, DEFAULT_DELIMETERS, false);
    for (int i = 0; i < sizeSplitted; i++){
        // wprintf(L"[%ls]\n", splittedSentence[i][0]);
        if (iswlower(splittedSentence[i][0])){
            return true;
            break;
        }
    }
    return false;
}

Text function2(Text text){
    int j = 0;
    for (int i = 0; i < text.size; i++){
        if (containsLowerFirstLetters(text.sentences[i]) == false){
            text.sentences[j++] = text.sentences[i];
        }
    }
    text.size = j;
    return text;
}

