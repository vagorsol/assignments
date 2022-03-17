#include <stdio.h>
#include <string.h>
#include <stdlib.h>

unsigned char scramble(char input){
  unsigned int bitoop = input & 0xF9;
  unsigned int bit2 = input & 0x4;
  unsigned int bit3 = input & 0x2;
  unsigned char ret; 
  bit2 = bit2 >> 1;
  bit3 = bit3 << 1;
  ret = (unsigned char) (bitoop | bit2 | bit3);
  return ret; 
}

int main() {
  char input[100]; 
  printf("Please enter a phrase: ");
  scanf(" %s", input);
  
  unsigned char* output;
  output = calloc(sizeof(unsigned char) * strlen(input), strlen(input) + 1);  
  // loop through all the characters in the given string
  for(int i = 0; i < strlen(input); i++){
    output[i] = scramble(input[i]);  
  }
  printf("scramble: %s\n", output);
  free(output);
  output = NULL;
  return 0;
}
