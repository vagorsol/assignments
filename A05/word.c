/* word.c
 * Author: Audrey Yang
 * Date: 2/20/2022
 * Given a (valid) four letter word, shift the hex value to the corresponding
 * "place" in the word, and then convert the word into a single, unsigned 
 * integer (hex -> decimal).
 */
#include <stdio.h>
#include <stdlib.h>

int main() {
  char word[4];
  printf("Enter 4 characters: ");
  scanf(" %s", word);
  
  unsigned int sum = 0; // the converted number at the end
  int atoiied; // the int value of the char 
  for(int i = 0; i < 4; i++){ 
    atoiied = word[4 - i - 1];
    // "shift" the bit appropriately
    if(i != 0){
      for(int j = 0; j < i; j++){
        atoiied = atoiied * 256;
      } 
    }
    sum = sum + atoiied;
    // printf("Converted Int of %c: %d\n",  word[i], atoiied);
    printf("Debug: %c = 0x%08X\n", word[4 - i - 1], atoiied);
  }  

  printf("Your number is: %u (0x%08X)\n", sum, sum);
  return 0;
}
