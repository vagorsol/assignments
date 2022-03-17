#include <stdio.h>
#include <string.h>
#include <stdlib.h>

unsigned char scramble(char input){
  unsigned int bit1 = input & 0x8;
  unsigned int bit2 = input & 0x4;
  unsigned int bit3 = input & 0x2;
  unsigned int bit4 = input & 0x1;
  unsigned char ret; 
  // TODO: doesn't return right character
  bit2 = bit2 >> 1;
  bit3 = bit3 << 1;
  ret = (unsigned char) (bit1 | bit2 | bit3 | bit4);
  // printf("Input: %X Output: %X\n", input, ret);
  return ret; 
}

int main() {
  char input[100]; 
  printf("Please enter a phrase: ");
  scanf(" %s", input);
  
  char* output;
  output = calloc(sizeof(char) * strlen(input), strlen(input) + 1);  
  // loop through all the characters in the given string
  for(int i = 0; i < strlen(input); i++){
    output[i] = scramble(input[i]);  
    printf("%d", output[i]);
  }
  printf("scramble: %s\n", output);
  free(output);
  output = NULL;
  return 0;
}
