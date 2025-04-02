// Program Name: Shortest Word Ladder
// Author: Jose Ramirez
// Last Updated: 04/02/2025 (1.1.0)
// Description: Finds the shortest word ladders between words using BFS algorithm.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

/* ========== DATA STRUCTURES ========== */

typedef struct WordNode_struct {
    char* myWord;
    struct WordNode_struct* next;
} WordNode;

typedef struct LadderNode_struct {
    WordNode* topWord;
    struct LadderNode_struct* next;
} LadderNode;

/* ===== Dictionary File Operations ===== */

/*  Count words of specific length in file
    @param filename - dictionary file name
    @param wordSize - target word length
    @return count of words with exact length, -1 if file error  */
int countWordsOfLength(char* filename, int wordSize) {
    FILE* file = fopen(filename, "r");
    if (!file) return -1;

    int count = 0;
    char buffer[256];

    while (fscanf(file, "%255s", buffer) == 1) {
        if (strlen(buffer) == wordSize) {
            count++;
        }
    }
    fclose(file);
    return count;
}

/*  Populate pre-allocated words array with words of exact length
    @param filename - dictionary file name
    @param words - array to fill (pre-allocated)
    @param numWords - expected number of words
    @param wordSize - target word length
    @return true if file read successfullt with exact numWords  */
bool buildWordArray(char* filename, char** words, int numWords, int wordSize) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) return false;

    int index = 0;
    char buffer[256];

    while (fscanf(file, "%255s", buffer) == 1) {
        if (strlen(buffer) == wordSize) {
            if (index >= numWords) {
                fclose(file);
                return false;
            }
            strcpy(words[index], buffer);
            index++;
        }
    }
    fclose(file);
    return index == numWords;
}

/* ======= Word Array Operations ======= */

/*  Binary search for word in sorted array
    @param words - sorted dictionary array
    @param aWord - target word to find
    @param loInd - search range start index
    @param hiInd - search range start index
    @return index is found, -1 otherwise  */
int findWord(char** words, char* aWord, int loInd, int hiInd) {
    while (loInd <= hiInd) {
        int mid = loInd + (hiInd - loInd) / 2;
        int cmp = strcmp(words[mid], aWord);
        if (cmp == 0) {
            return mid;
        } else if (cmp < 0) {
            loInd = mid + 1;
        } else {
            hiInd = mid - 1;
        }
    }
    return -1;
}

/*  Free memory for words array
    @param words - array of strings to free
    @param numWords - number of elements in array  */
void freeWords(char** words, int numWords) {
    if (words == NULL) return;
    for (int i = 0; i < numWords; i++) {
        free(words[i]);
    }
    free(words);
}

/* ====== Word Ladder Operations ======= */

/*  Insert new word at front of ladder (modifies head pointer)
    @param ladder - pointer to ladder head pointer
    @param newWord - word to add to front  */
void insertWordAtFront(WordNode** ladder, char* newWord) {
    WordNode* newNode = (WordNode*)malloc(sizeof(WordNode));
    newNode -> myWord = newWord;
    newNode -> next = *ladder;
    *ladder = newNode;
}

/*  Calculate number of words in ladder
    @param ladder - head of ladder list
    @return count of nodes in ladder  */
int getLadderHeight(WordNode* ladder) {
    int count = 0;
    WordNode* current = ladder;
    while (current != NULL) {
        count++;
        current = current -> next;
    }
    return count;
}

/*  Create deep copy of ladder structure (shallow copy of words)
    @param ladder - ladder to duplicate
    @return new independent ladder with same words  */
WordNode* copyLadder(WordNode* ladder) {
    WordNode* newLadder = NULL;
    WordNode** current = &newLadder;
    WordNode* original = ladder;
    while (original != NULL) {
        *current = (WordNode*)malloc(sizeof(WordNode));
        (*current) -> myWord = original -> myWord;
        (*current) -> next = NULL;
        current = &((*current) -> next);
        original = original -> next;
    }
    return newLadder;
}

/*  Free all nodes in ladder (doesn't free actual words)
    @param ladder - head of ladder to destroy  */
void freeLadder(WordNode* ladder) {
    WordNode* current = ladder;
    while (current != NULL) {
        WordNode* next = current -> next;
        free(current);
        current = next;
    }
}

/* = Ladder List (BFS QUEUE) Operations = */

/*  Add new ladder to end of ladder list (BFS queue)
    @param list - pointer to head of ladder list
    @param newLadder - ladder to append  */
void insertLadderAtBack(LadderNode** list, WordNode* newLadder) {
    LadderNode* newNode = (LadderNode*)malloc(sizeof(LadderNode));
    newNode -> topWord = newLadder;
    newNode -> next = NULL;

    if (*list == NULL) {
        *list = newNode;
    } else {
        LadderNode* current = *list;
        while (current -> next != NULL) {
            current = current -> next;
        }
        current -> next = newNode;
    }
}

/*  Remove and return first ladder from list (BFS dequeue)
    @param list - pointer to head of ladder list
    @return removed ladder (nodes preserved), NULL is empty  */
WordNode* popLadderFromFront(LadderNode** list) {
    if (*list == NULL) return NULL;

    LadderNode* oldHead = *list;
    WordNode* result = oldHead -> topWord;
    *list = oldHead -> next;
    free(oldHead);
    return result;
}

/*  Free entire ladder list and all contained ladders
    @param myList - head of ladder list to destroy  */
void freeLadderList(LadderNode* myList) {
    while (myList != NULL) {
        LadderNode* next = myList -> next;
        freeLadder(myList -> topWord);
        free(myList);
        myList = next;
    }
}

/*  Calculate number of ladders in list
    @param list - head of ladder list
    @return count of ladders in queue  */
int getLadderListSize(LadderNode* list) {
    int size = 0;
    LadderNode* current = list;
    while (current != NULL) {
        size++;
        current = current -> next;
    }
    return size;
}

/* =========== Core Algorithm =========== */

/*  BFS-based shotest path finder between words
    @param words - dictionary array
    @param usedWord - tracking array for used words
    @param numWords - total words in dictionary
    @param wordSize - length of all words
    @param startWord - beginning of ladder
    @param finalWord - end of ladder
    @return shortest ladder (reversed order), NULL if nore  */
WordNode* findShortestWordLadder(char** words, bool* usedWord, int numWords,
                                 int wordSize, char* startWord, char* finalWord ) {
    int startIndex = findWord(words, startWord, 0, numWords - 1);
    if (startIndex == -1 || usedWord[startIndex]) return NULL;

    int finalIndex = findWord(words, finalWord, 0, numWords - 1);
    if (finalIndex == -1 || usedWord[finalIndex]) return NULL;

    if (startIndex == finalIndex) return NULL;

    WordNode* initialLadder = NULL;
    insertWordAtFront(&initialLadder, words[startIndex]);
    usedWord[startIndex] = true;

    LadderNode* queue = NULL;
    insertLadderAtBack(&queue, initialLadder);

    while (queue != NULL) {
        int levelSize = getLadderListSize(queue);

        for (int i = 0; i < levelSize; i++) {
            WordNode* currentLadder = popLadderFromFront(&queue);
            char* currentWord = currentLadder -> myWord;

            if (strcmp(currentWord, words[finalIndex]) == 0) {
                freeLadderList(queue);
                return currentLadder;
            }

            for (int j = 0; j < numWords; j++) {
                if (usedWord[j]) continue;

                int diff = 0;
                for (int k = 0; k < wordSize; k++) {
                    if (currentWord[k] != words[j][k]) {
                        diff++;
                        if (diff > 1) break;
                    }
                }

                if (diff == 1) {
                    usedWord[j] = true;
                    WordNode* newLadder = copyLadder(currentLadder);
                    insertWordAtFront(&newLadder, words[j]);
                    insertLadderAtBack(&queue, newLadder);
                }
            }

            freeLadder(currentLadder);
        }
    }

    freeLadderList(queue);
    return NULL;
}

/* ====== User Interface & Testing ====== */

// interactive user-input to set a word;
//  ensures the word is in the dictionary word array
void setWord(char** words, int numWords, int wordSize, char* aWord) {
    bool valid = false;
    printf("  Enter a %d-letter word: ", wordSize);
    int count = 0;
    while (!valid) {
        scanf("%s",aWord);
        count++;
        valid = (strlen(aWord) == wordSize);
        if (valid) {
            int wordInd = findWord(words, aWord, 0, numWords-1);
            if (wordInd < 0) {
                valid = false;
                printf("    Entered word %s is not in the dictionary.\n",aWord);
                printf("  Enter a %d-letter word: ", wordSize);
            }
        } else {
            printf("    Entered word %s is not a valid %d-letter word.\n",aWord,wordSize);
            printf("  Enter a %d-letter word: ", wordSize);
        }
        if (!valid && count >= 5) { //too many tries, picking random word
            printf("\n");
            printf("  Picking a random word for you...\n");
            strcpy(aWord,words[rand()%numWords]);
            printf("  Your word is: %s\n",aWord);
            valid = true;
        }
    }
}

// helpful debugging function to print a single Ladder
void printLadder(WordNode* ladder) {
    WordNode* currNode = ladder;
    while (currNode != NULL) {
        printf("\t\t\t%s\n",currNode->myWord);
        currNode = currNode->next;
    }
}

// helpful debugging function to print the entire list of Ladders
void printList(LadderNode* list) {
    printf("\n");
    printf("Printing the full list of ladders:\n");
    LadderNode* currList = list;
    while (currList != NULL) {
        printf("  Printing a ladder:\n");
        printLadder(currList->topWord);
        currList = currList->next;
    }
    printf("\n");
}

/* ============ Main Program ============ */

//-----------------------------------------------------
// The primary application is fully provided in main();
//  no changes should be made to main(), except for
//  testing-related purposes (such as command-line
//  arguments for "TESTING MODE" to call a test case
//  master function, or something similar)
//-----------------------------------------------------
int main() {
    srand((int)time(0));

    printf("\nWelcome to the CS 211 Word Ladder Generator!\n\n");

    // set word length using interactive user-input
    int wordSize;
    printf("Enter the word size for the ladder: ");
    scanf("%d",&wordSize);
    printf("\n");

    printf("This program will make the shortest possible\n");
    printf("word ladder between two %d-letter words.\n\n",wordSize);

    // interactive user-input sets the dictionary file;
    //  check that file exists; if not, user enters another file
    //  if file exists, count #words of desired length [wordSize]
    char dict[100];
    printf("Enter filename for dictionary: ");
    scanf("%s", dict);
    printf("\n");
    int numWords = countWordsOfLength(dict,wordSize);
    while (numWords < 0) {
        printf("  Dictionary %s not found...\n",dict);
        printf("Enter filename for dictionary: ");
        scanf("%s", dict);
        printf("\n");
        numWords = countWordsOfLength(dict,wordSize);
    }

    // allocate heap memory for the word array; only words with desired length
    char** words = (char**)malloc(numWords*sizeof(char*));
    for (int i = 0; i < numWords; ++i) {
        words[i] = (char*)malloc((wordSize+1)*sizeof(char));
    }

    // end program if file does not have at least two words of desired length
    if (numWords < 2) {
        printf("  Dictionary %s contains insufficient %d-letter words...\n",dict,wordSize);
        printf("Terminating program...\n");
        return -1;
    }

    // [usedWord] bool array has same size as word array [words];
    //  all elements initialized to [false];
    //  later, usedWord[i] will be set to [true] whenever
    //      words[i] is added to ANY partial word ladder;
    //      before adding words[i] to another word ladder,
    //      check for previous usage with usedWord[i]
    bool* usedWord = (bool*)malloc(numWords*sizeof(bool));
    for (int i = 0; i < numWords; ++i) {
        usedWord[i] = false;
    }

    // build word array (only words with desired length) from dictionary file
    printf("Building array of %d-letter words... ", wordSize);
    bool status = buildWordArray(dict,words,numWords,wordSize);
    if (!status) {
        printf("  ERROR in building word array.\n");
        printf("  File not found or incorrect number of %d-letter words.\n",wordSize);
        printf("Terminating program...\n");
        return -1;
    }
    printf("Done!\n");

    // set the two ends of the word ladder using interactive user-input
    //  make sure start and final words are in the word array,
    //  have the correct length (implicit by checking word array), AND
    //  that the two words are not the same
    char startWord[30];
    char finalWord[30];
    printf("Setting the start %d-letter word... \n", wordSize);
    setWord(words, numWords, wordSize, startWord);
    printf("\n");
    printf("Setting the final %d-letter word... \n", wordSize);
    setWord(words, numWords, wordSize, finalWord);
    while (strcmp(finalWord,startWord) == 0) {
        printf("  The final word cannot be the same as the start word (%s).\n",startWord);
        printf("Setting the final %d-letter word... \n", wordSize);
        setWord(words, numWords, wordSize, finalWord);
    }
    printf("\n");

    // run the algorithm to find the shortest word ladder
    WordNode* myLadder = findShortestWordLadder(words, usedWord, numWords, wordSize, startWord, finalWord);

    // display word ladder and its height if one was found
    if (myLadder == NULL) {
        printf("There is no possible word ladder from %s to %s\n",startWord,finalWord);
    } else {
        printf("Shortest Word Ladder found!\n");
        printLadder(myLadder);
    }
    printf("Word Ladder height = %d\n",getLadderHeight(myLadder));

    // free the heap-allocated memory for the shortest ladder
    freeLadder(myLadder);
    // free the heap-allocated memory for the words array
    freeWords(words,numWords);
    free(usedWord);

    return 0;
}
