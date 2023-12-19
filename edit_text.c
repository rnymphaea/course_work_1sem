#include "edit_text.h"
#define DEFAULT_DELIMETERS L" ,.-\n"
#define ALL_LETTERS L"QWERTYUIOPASDFGHJKLZXCVBNMqwertyuiopasdfghjklzxcvbnmЙЦУКЕНГШЩЗХЪФЫВАПРОЛДЖЭЯЧСМИТЬБЮйцукенгшщзхъфывапролджэячсмитьбю"

wchar_t * toLowerSentence(Sentence sentence){
    wchar_t * lowerSentence = (wchar_t *)malloc((wcslen(sentence.text) + 1) * sizeof(wchar_t));
    if (lowerSentence == NULL){
        wprintf(L"Error: cannot allocate memory!");
        return NULL;
    }
    wcscpy(lowerSentence, sentence.text);
    int length = wcslen(sentence.text);
    for (int i = 0; i < length; i++){
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
            free(curr_sent);
            return true;
            break;
        }
        free(curr_sent);
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
    wchar_t ** result = (wchar_t **)calloc(sentence.size + 1, sizeof(wchar_t *));
    wchar_t * currSentence  = (wchar_t *)calloc(sentence.size + 1, sizeof(wchar_t *));
    if (result == NULL || currSentence == NULL){
        wprintf(L"Error: cannot allocate memory!");
        exit(0);
    }
    if (lower == true){
        currSentence = toLowerSentence(sentence);
    }
    else {
        wcscpy(currSentence, sentence.text);
    }
    wchar_t * valueToken; // state of the current tokenization sequence
    wchar_t * currentWord;
    currentWord = wcstok(currSentence, delims, &valueToken);
    while (currentWord != NULL){
        result[(*size)++] = currentWord;
        currentWord = wcstok(NULL, delims, &valueToken);
    }
    // if (lower){
    //     free(currSentence);
    // }
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
            free(splittedSentence);
            return true;
            break;
        }
    }
    free(splittedSentence);
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

int compareByLength(const void * a, const void * b){
    wchar_t * str1 = *((wchar_t **)a);
    wchar_t * str2 = *((wchar_t **)b);
    int length1 = wcslen(str1);
    int length2 = wcslen(str2);
    if (length1 > length2){
        return 1;
    }
    else if (length1 < length2){
        return -1;
    }
    return 0;
}

int compareByLetters(const void * a, const void * b){
    wchar_t * mask1 = *((wchar_t **)a);
    wchar_t * mask2 = *((wchar_t **)b);
    int count1 = getCountLetters(mask1);
    int count2 = getCountLetters(mask2);
    if (count1 < count2){
        return 1;
    }
    else if (count1 > count2){
        return -1;
    }
    return 0;
}

int getCountLetters(wchar_t * mask){
    int len = wcslen(mask);
    int count = 0;
    for (int i = 0; i < len; i++){
        if (mask[i] != L'?' && mask[i] != L'*'){
            count++;
        }
    }
    return count;
}

wchar_t * getTrueMask(wchar_t ** masks, int sizeTmp){
    qsort(masks, sizeTmp, sizeof(wchar_t *), compareByLetters);
    return masks[0];
}

bool inWords(wchar_t symbol, wchar_t ** splitted, int sizeSplitted, int index){
    for (int i = index; i < sizeSplitted; i++){
        wchar_t * ptr = wcschr(splitted[i], symbol);
        if (ptr != NULL){
            continue;
        }
        else {
            return false;
            break;
        }
        // wprintf(L"%p", ptr);
    }
    // putwchar(symbol);
    
    return true;
}

wchar_t * getMask(Sentence sentence){
    int sizeSplitted = 0;
    wchar_t ** splittedSentence = getSplittedText(sentence, &sizeSplitted, DEFAULT_DELIMETERS, false);
    if (sizeSplitted == 1){
        return splittedSentence[0];
    }
    wchar_t ** result = (wchar_t **)calloc(30, sizeof(wchar_t *));
    
    int size = 0;
    wchar_t * resultMask = (wchar_t *)calloc(100, sizeof(wchar_t));
    qsort(splittedSentence, sizeSplitted, sizeof(wchar_t *), compareByLength);
    wcscpy(resultMask, splittedSentence[0]);
    int sizeMask = wcslen(resultMask);
    int size2 = wcslen(splittedSentence[1]);
    // wchar_t * currentMask = (wchar_t *)calloc(wcslen(result[0]) + 3, sizeof(wchar_t)); // + 2*"*" + '\0'
    wchar_t * mask;
    int currSize;
    
    for (int i = 0; i < size2 - sizeMask + 1; i++){
        mask = (wchar_t *)calloc(sizeMask + 100, sizeof(wchar_t));
        currSize = 0;
        if (i > 0){
            mask[currSize++] = '*';
        }
        for (int j = 0; j < sizeMask; j++){
            if (splittedSentence[0][j] == splittedSentence[1][j + i]){
                mask[currSize++] = splittedSentence[0][j];
            }
            else {
                mask[currSize++] = L'?';
            }
        }
        if (i != size2 - sizeMask){
            mask[currSize++] = L'*';
        }
        result[size++] = mask;
        
    }
    wchar_t * resTmp = getTrueMask(result, size);

    if (sizeSplitted == 2){
        return resTmp;
    }
    int tmp;
    int lengthResTmp = wcslen(resTmp);
    bool checkStart = (resTmp[0] == L'*');
    bool checkEnd = (resTmp[lengthResTmp - 1] == L'*');
    if (checkStart){
        int tmpSize = 0;
        for (int i = 1; i < lengthResTmp; i++){
            resTmp[tmpSize++] = resTmp[i];
        }
        lengthResTmp = tmpSize;
    }
    if (checkEnd){
        resTmp[--lengthResTmp] = L'\0';
        
    }
    for (int i = 2; i < sizeSplitted; i++){
        wchar_t ** masks = (wchar_t **)calloc(30, sizeof(wchar_t *));
        int currSizeMasks = 0;
        wchar_t * currWord = splittedSentence[i];
        int currWordSize = wcslen(currWord);
        for (int j = 0; j < currWordSize - lengthResTmp + 1; j++){ // смещение
            wchar_t * tmpMask = (wchar_t *)calloc(lengthResTmp + 1, sizeof(wchar_t));
            int tmpSize = 0;
            for (int k = 0; k < lengthResTmp; k++){
                if (resTmp[k] == currWord[k + j]){
                    tmpMask[tmpSize++] = resTmp[k];
                }
                else {
                    tmpMask[tmpSize++] = L'?';
                }
            }
            if (j != currWordSize - lengthResTmp){
                tmpMask[tmpSize++] = L'*';
            }
            masks[currSizeMasks++] = tmpMask;
        }
        resTmp = getTrueMask(masks, currSizeMasks);
    }
    if (checkStart){
        wchar_t * star = L"*";
        wchar_t * tmpStr = (wchar_t * )calloc(lengthResTmp + 2, sizeof(wchar_t));
        wcscat(tmpStr, star);
        wcscat(tmpStr, resTmp);
        return tmpStr;
    }
    return resTmp;
}
