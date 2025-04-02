# Project 04: Link Lint List - Find Shortest Word Ladder #
## Introduction To Word Ladders ##
A word ladder is a bridge between one word and another, formed by changing one letter at a time, with the constraint 
that at each step the sequence of letters still forms a valid word. For example, here is a word ladder starting from 
the word "data" and climbing up the ladder to the word "code". Each changed letter is bolded as an illustration:
    
    |  code  |
    |    ^   |
    |  cove  |  
    |   ^    |
    |  cave  |
    |  ^     |
    |  gave  |
    |    ^   |
    |  gate  |
    |  ^     |
    |  date  |
    |     ^  |
    |  data  |

This word ladder has height 7. There are many other word ladders that connect these two words. For example, here is 
another ladder of height 5 that uses some more obscure words:

    |  code  |
    |   ^    |
    |  cade  |  
    |    ^   |
    |  cate  |
    |  ^     |
    |  date  |
    |     ^  |
    |  data  |

In fact, this ladder has the shortest height that connects data to code. Note that there might be other ladders of the 
same height, but none with fewer rungs than this one. Notice that the shortest a word ladder can be is 2, e.g. cove -> 
code. There are also pairs of words for which a word ladder cannot be formed, e.g. there is no word ladder to connect 
stack -> queue.

This project consists of writing functions to support a program that prompts the user for two words and finds a 
minimum-height ladder linking the words, which uses linked lists of words to store ladders, linked lists of partially 
completed ladders to organize all possible solutions, and a prescribed algorithm to find the shortest word ladder.
___

## Dictionary File, Word Array, and a Linked List for the Word Ladder ##
The provided file dictionary.txt contains the full contents of the "Official Scrabble Player's Dictionary, Second 
Edition." This word list has over 120,000 words, which should be more than enough for the purposes of making word 
ladders for small to moderate sized words. Smaller dictionaries are also provided for testing purposes: simple3.txt 
contains a limited number of 3-letter words, simple4.txt contains a limited number of 4-letter words, and simple5.txt 
contains a limited number of 5-letter words.

The complete primary application is provided in main() of the starter code; however, it involves many calls to functions 
that still need to be written. Here are the main steps of the program:

- The user interactively sets the length of the starting and final words for the word ladder. This, in turn, also sets 
the wordSize for the full set of words to be read in from the dictionary file.
- The user also interactively inputs the dictionary file name to be used for reading words into the full array of 
- possible words that could later make up the ladder.
- The dictionary file is opened for the first time and is scanned to count the number of words it contains that has the 
desired wordSize; this count is stored as numWords.
- We now know the user-specified word length (wordSize) AND the number of words in the dictionary that have the correct 
length (numWords). Thus, we can now allocate array space for all of the words, open the file again, and read the words 
in from the file to fill the newly-allocated array. The full set of words is stored using a heap-allocated array of 
pointers to C-strings. That is, words is an array of pointers with size numWords, where each pointer points to a 
heap-allocated C-string of size wordSize+1 (to allow space for a word and the null character). See the figure below for 
a diagram of the words array.
- The user interactively inputs both the starting word (startWord) and the final word (finalWord). If either entered 
word has an incorrect size (i.e. not equal to wordSize) or the word is not found in the words array, then the user is 
requested to enter another word.
- With the words array filled from the dictionary and the two ends of the word ladder set, an algorithm is run to 
produce the minimum-height word ladder. This entails building a linked list of WordNode structs, which contain pointers 
to C-strings in the words array and a pointer to the next element in the linked list. Full details on the minimum-height 
word ladder algorithm are in the next section.
- If a word ladder connecting the two words is possible, the minimum-height ladder is displayed (with the starting word 
on the bottom and the final word at the top of the ladder), along with the ladder height.