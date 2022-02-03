/*
 * wordguess.c
 * Author: Audrey Yang
 * Date: 2/2/2022
 * Like Wordle, but less cool and without the colors. 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// check if syntax is correct
// prints out the current word
void printCurrentWordStatus(char *wordToPrint, int wordLen){
  for(int i = 0; i < wordLen; i++){
      printf("%c ", wordToPrint[i]);
  }
  printf("\n");
}

// checks if the given letter is in the word
// if it is, update the string and return true. else, return false
int letterCheck(char *word, char *playerWord, char guess, int len){
  int ret = 0; // default as "false"
  
  // loop through the word, checking if that letter is in there
  for(int i = 0; i < len; i++){
    // if the guessed letter is in the word, update the word
    if(word[i] == guess){
      playerWord[i] = guess; // update playerWord
      ret = 1; // set as true
      // printCurrentWordStatus(playerWord, len);
    }
  }
  return ret; 
}

// comapres two string up to a given string length
// essentially "trims" word and then compares it with playerWord
// returns 0 if they are not equal, and 1 if they are
int strCmpTrimmed(char *word, char *playerWord, int len){
  for(int i = 0; i < len; i++){
    if(playerWord[i] != word[i]){
      return 0;
    }
  }
  return 1;
}

int main() {
  char wordToGuess[32]; // the word
  char *playerCurrWord; // player revealed word
  char playerGuess; // what the player inputs as their guess
  int turns = 1; // keeps track of the number of turns
  int win = 0; // 'bool' value that tracks whether player has won or not
  int wordLen; // length of the word to guess
  int letterStatus; // used to check if the user guesses a letter in the word  
  char ch[32]; // the # of words in the file 
  int randWord; // the line # of the random word to guess
  FILE *infile; // pointer to the file with the words
  
  // initialize random number generator
  srand(time(0));  
   
  // open and read the file with the words
  infile = fopen("words.txt", "r"); 
  if(infile == NULL){
    printf("Unable to access word bank %s!\n","txt");
    exit(1);
  }
   
  // get the number of words in the file
  //ch = getc(infile);
  fgets(ch, 32, infile); // get whole line
  if(ch != NULL) {
     // convert the ascii of ch to an int
     // char chConversion = ch;
     // int wordCount = chConversion - '0'; 
     int wordCount = atoi(ch); 
     randWord = rand() % wordCount;
     randWord++; // increase by one since line 0 is not a legitimate option
     // printf("Word Index: %d\n", randWord);
  } else {
    // if there's nothing in the file, the just exit
    printf("Not enough words to choose from!\n");
    exit(1);
  }
  
  int itt = 0;
  while(itt < randWord){
    // if the program can't read the next line, error message
    if(fgets(wordToGuess, 32, infile) == NULL){
      printf("Failed to scroll through the file.\n");
      exit(1); 
    } else{
      // printf("Current Word is: %s\n", wordToGuess);
      itt++;
    }
  }

  wordLen = strlen(wordToGuess) - 1; 
  playerCurrWord = malloc(sizeof(char) * wordLen);  
  // check that initialization went well
  if(playerCurrWord == NULL){
    printf("Failed to pick a word!\n");
    exit(1);
  }
    
  // make all of currword be "_"
  for(int i = 0; i < wordLen; i++){
      strcpy(&playerCurrWord[i], "_");
  }
  // printf("Word Start: %s\n", playerCurrWord);
  // close the file
  fclose(infile);

  printf("Welcome to Word Guess!\n");
  // printf("Word (FOR DEBUGGING PURPOSES ONLY): %s\n", wordToGuess);

  // word guessing
  while(win != 1){
    printf("Turn: %d\n", turns);

    // print out word so far
    // printf("word: %s\n",playerCurrWord);
    printCurrentWordStatus(playerCurrWord, wordLen);
    
    // get player's guess 
    printf("Guess a character: ");
    scanf(" %c", &playerGuess); 
  
    // check to see if the guessed letter is in the word
    letterStatus = letterCheck(wordToGuess, playerCurrWord, playerGuess, wordLen); 

    if(letterStatus != 0){
      // check to see if the player won or not
      if(strCmpTrimmed(wordToGuess, playerCurrWord, wordLen) == 1){
        // if the player won, update win condition (and stop the loop)
        win = 1;
      } else{
        // otherwise, increase the number of turns
         turns++;
      } 
    } else{
      printf("Sorry! No %c found!\n", playerGuess);
      turns++;
    }

    printf("\n");
  }
  
  // print out the word one more time 
  printCurrentWordStatus(playerCurrWord, wordLen);
  printf("You won in %d turn!\n", turns);

  free(playerCurrWord);
  playerCurrWord = NULL;
  return 0;
}
