#include "headers/edit_text.h"
#include "headers/io_functions.h"
#include "headers/defines.h"

wchar_t * toLowerSentence(Sentence sentence){
    wchar_t * lowerSentence = (wchar_t *)malloc((wcslen(sentence.text) + 1) * sizeof(wchar_t));
    if (lowerSentence == NULL){
        printMemoryError();
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
        printMemoryError();
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
    wchar_t * strOne = *((wchar_t **)a);
    wchar_t * strTwo = *((wchar_t **)b);
    int countFirst = getCountVowels(strOne);
    int countSecond = getCountVowels(strTwo);
    if (countFirst > countSecond){
        return 1;
    }
    else if (countFirst < countSecond){
        return -1;
    }
    return 0;
}

int getCountVowels(wchar_t * word){
    int size;
    size = wcslen(word);
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
    const wchar_t vowels[] = LOWER_LETTERS;
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
    int len = wcslen(destination);
    destination[len] = END_OF_STRING;
    return destination;
}

Text getSortedText(Text text){
    for (int i = 0; i < text.size; i++){
        int sizeSplitted = 0;
        int sizeDelims = 0;
        wchar_t ** splittedSentence = getSplittedText(text.sentences[i], &sizeSplitted, DEFAULT_DELIMETERS, false);
        wchar_t ** delims = getSplittedText(text.sentences[i], &sizeDelims, ALL_LETTERS, false);
        wchar_t * resultSentence = (wchar_t *)calloc(text.sentences[i].size + BUF_SIZE, sizeof(wchar_t *));
        if (sizeSplitted == 0 || sizeDelims == 0){
            continue;
        }
        qsort(splittedSentence, sizeSplitted, sizeof(wchar_t *), compareByVowels);
        resultSentence = makeSentence(resultSentence, splittedSentence, delims, sizeSplitted);
        text.sentences[i].text = resultSentence;
    }
    return text;
}

int compareByLength(const void * a, const void * b){
    wchar_t * FirstStr = *((wchar_t **)a);
    wchar_t * SecondStr = *((wchar_t **)b);
    int lengthFirst = wcslen(FirstStr);
    int lengthSecond = wcslen(SecondStr);
    if (lengthFirst > lengthSecond){
        return 1;
    }
    else if (lengthFirst < lengthSecond){
        return -1;
    }
    return 0;
}

int compareByLetters(const void * a, const void * b){
    wchar_t * FirstMask = *((wchar_t **)a);
    wchar_t * SecondMask = *((wchar_t **)b);
    int countFirst = getCountLetters(FirstMask);
    int countSecond = getCountLetters(SecondMask);
    if (countFirst < countSecond){
        return 1;
    }
    else if (countFirst > countSecond){
        return -1;
    }
    return 0;
}

int getCountLetters(wchar_t * mask){
    int len = wcslen(mask);
    int count = 0;
    for (int i = 0; i < len; i++){
        if (mask[i] != INTERROBANG && mask[i] != ASTERISK){
            count++;
        }
    }
    return count;
}

wchar_t * getTrueMask(wchar_t ** masks, int sizeTmp){
    qsort(masks, sizeTmp, sizeof(wchar_t *), compareByLetters);
    return masks[0];
}

bool isAllMissed(wchar_t * mask){
    int len = wcslen(mask);
    for (int i = 0; i < len; i++){
        if (iswalnum(mask[i])){
            return false;
            break;
        }
    }
    return true;
}

wchar_t * getStartMask(wchar_t ** splittedSentence){
    int currBuf = BUF_SIZE;
    wchar_t ** result = (wchar_t **)calloc(BUF_SIZE, sizeof(wchar_t *));
    int size = 0;
    int sizeMask = wcslen(splittedSentence[0]);
    int size2 = wcslen(splittedSentence[1]);

    wchar_t * mask;
    int currSize;
    
    for (int i = 0; i < size2 - sizeMask + 1; i++){
        mask = (wchar_t *)calloc(sizeMask + 3, sizeof(wchar_t));
        currSize = 0;
        if (i > 0){
            mask[currSize++] = ASTERISK;
        }
        for (int j = 0; j < sizeMask; j++){
            if (splittedSentence[0][j] == splittedSentence[1][j + i]){
                mask[currSize++] = splittedSentence[0][j];
            }
            else {
                mask[currSize++] = INTERROBANG;
            }
        }
        if (i != size2 - sizeMask){
            mask[currSize++] = ASTERISK;
        }
        result[size++] = mask;
        if (size == currBuf - 1){
            currBuf += BUF_SIZE;
            wchar_t ** tmpPtr = realloc(result, currBuf);
            if (tmpPtr != NULL){
                result = tmpPtr;
            }
            else {
                printMemoryError();
            }
        }
    }
    wchar_t * res = getTrueMask(result, size);
    return res;
}

wchar_t * getMask(Sentence sentence){
    int sizeSplitted = 0;
    wchar_t ** splittedSentence = getSplittedText(sentence, &sizeSplitted, DEFAULT_DELIMETERS, false);

    if (sizeSplitted < 2){
        return sentence.text;
    }

    qsort(splittedSentence, sizeSplitted, sizeof(wchar_t *), compareByLength);

    wchar_t * resTmp = getStartMask(splittedSentence); // стартовая маска
    
    int lengthResTmp = wcslen(resTmp);

    if (sizeSplitted == 2){
        if (isAllMissed(resTmp)){
            return STRING_ASTERISK;
        }
        else {
            resTmp[lengthResTmp] = END_OF_STRING;
            return resTmp;
        }
    }
    
    bool checkEnd = (resTmp[lengthResTmp - 1] == ASTERISK);
    if (checkEnd){
        resTmp[--lengthResTmp] = END_OF_STRING;
    }
    wchar_t * tmpMask;
    wchar_t ** masks;
    for (int i = 2; i < sizeSplitted; i++){
        masks = (wchar_t **)calloc(BUF_SIZE, sizeof(wchar_t *));
        int currBufSize = BUF_SIZE;
        int currSizeMasks = 0;
        wchar_t * currWord = splittedSentence[i];
        int currWordSize = wcslen(currWord);
        for (int j = 0; j < currWordSize - lengthResTmp + 1; j++){ // смещение
            tmpMask = (wchar_t *)calloc(lengthResTmp + 2, sizeof(wchar_t));
            int tmpSize = 0;
            for (int k = 0; k < lengthResTmp; k++){
                if (resTmp[k] == currWord[k + j]){
                    tmpMask[tmpSize++] = resTmp[k];
                }
                else {
                    tmpMask[tmpSize++] = INTERROBANG;
                }
            }
            if (j != currWordSize - lengthResTmp){
                tmpMask[tmpSize++] = ASTERISK;
            }
            masks[currSizeMasks++] = tmpMask;
            if (currSizeMasks == currBufSize - 1){
                currBufSize += BUF_SIZE;
                wchar_t ** tmpPtr = realloc(masks, currBufSize);
                if (tmpPtr != NULL){
                    masks = tmpPtr;
                }
                else {
                    printMemoryError();
                }
            }
        }
        
        resTmp = getTrueMask(masks, currSizeMasks);
    }
    bool checkStart = (resTmp[0] == ASTERISK);
 
    if (isAllMissed(resTmp)){
        return STRING_ASTERISK;
    }
    if (checkStart){
        wchar_t * strAsterisk = STRING_ASTERISK;
        wchar_t * tmpStr = (wchar_t * )calloc(lengthResTmp + 2, sizeof(wchar_t));

        wcscat(tmpStr, strAsterisk);
        wcscat(tmpStr, resTmp);
        int len = wcslen(tmpStr);
        tmpStr[len] = END_OF_STRING;
        return tmpStr;
    }
    if (resTmp[0] == INTERROBANG){
        resTmp[0] = ASTERISK;
    }
    resTmp[lengthResTmp + 1] = END_OF_STRING;
    return resTmp;
}

wchar_t * getStartWord(Text text){
    int size = 0;
    wchar_t ** splittedFirstSentence = getSplittedText(text.sentences[0], &size, DEFAULT_DELIMETERS, true);
    wchar_t * startWord = splittedFirstSentence[0];
    return startWord;
}

wchar_t * toLowerWord(wchar_t * word){
    int len = wcslen(word);
    wchar_t * copy = (wchar_t *)calloc(len + 1, sizeof(wchar_t));
    wcscpy(copy, word);
    for (int i = 0; i < len; i++){
        copy[i] = towlower(copy[i]);
    }
    copy[len] = END_OF_STRING;
    return copy;
}

int getCountLettersWord(wchar_t * word, wchar_t letter){
    letter = towlower(letter);
    int len = wcslen(word);
    wchar_t * copy = toLowerWord(word);
    int count = 0;
    
    for (int i = 0; i < len; i++){
        if (copy[i] == letter){
            count++;
        }
    }
    return count;
}

bool isTrue(wchar_t * startWord, wchar_t * word){
    int lenStart = wcslen(startWord);
    int lenEnd = wcslen(word);
    int countEnd = 0;
    int countStart = 0;
    for (int i = 0; i < lenEnd; i++){
        countEnd = getCountLettersWord(word, word[i]);
        countStart = getCountLettersWord(startWord, word[i]);
        if (countStart == 0){
            return false;
        }
    }
    return true;
}

void printTrueWords(Text text, wchar_t * startWord){
    int check = 0;
    for (int sentenceNum = 0; sentenceNum < text.size; sentenceNum++){
        int currSentenceSize = 0;
        wchar_t ** splittedSentence  = getSplittedText(text.sentences[sentenceNum], &currSentenceSize, DEFAULT_DELIMETERS, false);
        if (sentenceNum == 0){
            check = 1;
        }
        else {
            check = 0;
        }
        for (int wordNum = check; wordNum < currSentenceSize; wordNum++){
            if (isTrue(startWord, splittedSentence[wordNum])){
                wprintf(L"%ls\n", splittedSentence[wordNum]);
            }
        }
    }
}






