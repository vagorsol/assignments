/* binarychar.c
 * Author: Audrey Yang
 * Date: 2/20/2022
 * Given a (valid) 8-bit binary number, return the corresponding (string)
 * character (i.e., 0110001 is "a")
 */
#include <stdio.h>
#include <stdlib.h>

// being able to use the math.h exponent function requires some linking thing
// so instead i'm making my own function
int powerOf(int base, int power){
  if(power == 0){
    return 1;
  }
  int ret = 1;  
  for(int i = 0; i < power; i++){
    ret = ret * base;
  }
  return ret;
}

int main() {
  char binaryval[8];
  
  printf("Enter 8 bits: ");
  scanf(" %s", binaryval);

  int binaryint;
  int decimalval = 0;
  for(int i = 0; i < 8; i++){
    binaryint = binaryval[i] - '0';
    // printf("%c's value: %d\n", binaryval[i], binaryint);
   
    // binary to decimal conversion 
    int timesten = powerOf(2, 8 - i - 1);
    decimalval = decimalval + (binaryint * timesten); 
    // printf("current sum: %d\tpower: %d\n", decimalval, timesten); 
 }
  printf("Your character is: %c\n", decimalval);
  return 0;
}
