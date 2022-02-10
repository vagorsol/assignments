/*
 * repeat.c
 * Author: Audrey Yang
 * Date: 2/1/2022
 * Asks users for a string, and then repeats it a user-given amount of times
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main() {
  char userWord[32]; // word to repeat
  int size; // size of the word to repeat
  int itt; // stores # of times to repeat the word
  char *wordRepeated; // string that holds the word but repeated

  // get the word
  printf("Enter a word: ");
  scanf(" %32s", userWord); // also am hard capping length @ 32 char here
  // get the number of repeats
  printf("Enter a count: ");
  scanf(" %d",& itt);
  
  // get the size of the string to repeat
  size = strlen(userWord) + 1;
  userWord[size] = '\0';
  // printf("%s\n", userWord);

  // allocate space/size for the repeated word
  wordRepeated = malloc(sizeof(char) * size * itt); 
  if(wordRepeated == NULL){ 
    // if the user wants to repeat the word too many times for compiler to handle
    // or if allocating failed, error message
    printf("Cannot allocate new string. Exiting...\n");
    exit(1);
  }

  // initialize wordRepeated
  strcpy(wordRepeated, userWord);
  // making the repeated word string
  for(int i = 0; i < itt - 1; i++){
    // append the word to the string of the word repeated
    strncat(wordRepeated, userWord, size); 
  }
  
  printf("Your word is: %s\n", wordRepeated);

  free(wordRepeated);
  wordRepeated = NULL;
  return 0;
}
