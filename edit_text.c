#include "edit_text.h"
#define DEFAULT_DELIMETERS L" ,.-\n"
#define ALL_LETTERS L"QWERTYUIOPASDFGHJKLZXCVBNMqwertyuiopasdfghjklzxcvbnmЙЦУКЕНГШЩЗХЪФЫВАПРОЛДЖЭЯЧСМИТЬБЮйцукенгшщзхъфывапролджэячсмитьбю"

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
        if (iswlower(splittedSentence[i][0])){
            return true;
            break;
        }
    }
    return false;
}

Text deleteSentences(Text text){
    // предложения удаляются путём сдвига значений массива на одну позицию влево
    int tmpIndex = 0; 
    for (int i = 0; i < text.size; i++){
        if (containsLowerFirstLetters(text.sentences[i]) == false){ // если первая буква слова не в нижнем регистре, то помещаем элемент в новое место
            text.sentences[tmpIndex++] = text.sentences[i];
        }
        else {
            free(text.sentences[i].text);
        }
    }
    text.size = tmpIndex;
    return text;
}

int compareByVowels(const void * a, const void * b){
    wchar_t * str1 = *((wchar_t **)a);
    wchar_t * str2 = *((wchar_t **)b);
    int countFirst = getCountVowels(str1);
    int countSecond = getCountVowels(str2);
    if (countFirst > countSecond){
        return 1;
    }
    else if (countFirst < countSecond){
        return -1;
    }
    return 0;
}

int getCountVowels(wchar_t * word){
    int size = wcslen(word);
    int countVowels = 0;
    for (int i = 0; i < size; i++){
        if (isVowel(word[i]) == true){
            countVowels++;
        }
    }
    
    return countVowels;
}

bool isVowel(wchar_t symbol){
    wchar_t new = towlower(symbol);
    const wchar_t vowels[] = L"eyuioaуеыаоэяиью";
    wchar_t * ptr = wcsrchr(vowels, new);
    if (ptr != NULL){
        return true;
    }
    return false;
}

wchar_t * makeSentence(wchar_t * destination, wchar_t ** splittedSentence, wchar_t ** delims, int sizeSplitted){
    for (int i = 0; i < sizeSplitted; i++){
        wcscat(destination, splittedSentence[i]);
        wcscat(destination, delims[i]);
    }
    return destination;
}

Text getSortedText(Text text){
    for (int i = 0; i < text.size; i++){
        int sizeSplitted = 0;
        int sizeDelims = 0;
        wchar_t ** splittedSentence = getSplittedText(text.sentences[i], &sizeSplitted, DEFAULT_DELIMETERS, false);
        wchar_t ** delims = getSplittedText(text.sentences[i], &sizeDelims, ALL_LETTERS, false);
        wchar_t * resultSentence = (wchar_t *)malloc(text.sentences[i].size * sizeof(wchar_t *) + 1);
        qsort(splittedSentence, sizeSplitted, sizeof(wchar_t *), compareByVowels);
        resultSentence = makeSentence(resultSentence, splittedSentence, delims, sizeSplitted);
        text.sentences[i].text = resultSentence;
    }
    return text;
}


