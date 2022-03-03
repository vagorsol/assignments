/*
 * decode.c
 * Author: Audrey Yang
 * Date: 3/2/2022
 * Decryptes a message hidden in a PPM file.
 */

#include <stdio.h>
#include <stdlib.h>
#include "read_ppm.h"

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

int main(int argc, char** argv) {
  struct ppm_pixel *pixels;
  int width, height, maxchar;
  
  if(argv[1] == NULL){
    printf("No file to open!\n");
    exit(1);
  }
  
  pixels = read_ppm(argv[1], &width, &height);
  if(pixels == NULL){
    printf("ERROR: Cannot open the file: %s\n", argv[1]);
    exit(1);
  }
  maxchar = (width * height * 3) / 8; 
  
  printf("Reading %s with width %d and height %d\n", argv[1], width, height);
  printf("Max number of characters in the image: %d\n", maxchar);

  int *secretchar;
  secretchar = malloc(sizeof(int) * maxchar * 8);
  if(secretchar == NULL){
    printf("ERROR: unable to write secret message!\n");
    exit(1);
  }
  
  unsigned int mask = 0x1;
  int indx; // 2d array indexing
  int count = 0; // letter array indexing

  // take each least significant bit from each color value
  // then put it in and read 
  for(int i = 0; i < height; i++){
    for(int j = 0; j < width; j++){
      indx = i * width + j; 

      // get all the least significant bits
      // the masks are not right. why.
      secretchar[count] = mask & pixels[indx].red;
      secretchar[count + 1] = mask & pixels[indx].green;
      secretchar[count + 2] = mask & pixels[indx].blue;
      count = count + 3;
    }
  }

  char *secretmsg = NULL;
  secretmsg = calloc(maxchar, sizeof(char));
  if(secretmsg == NULL){
    printf("ERROR: unable to write secret message!\n");
    exit(1);
  }

  int secretletter = 0;
  count = 0;
  indx = 0;
  // read bits, detect null char
  for(int i = 0; i < maxchar * 8; i++){
    if(count == 8){
      secretmsg[indx] = secretletter; // add to arr

      if(secretletter == 0){
        // if null terminator - stop
        break; 
      } else{
        secretletter = 0;
        indx = indx + 1;
        count = 0;
      }
    } 
    // convert bit to binary and add to current letter
    secretletter = secretletter + (secretchar[i] * powerOf(2,7 - count)); 
    count = count + 1;
  }
  printf("%s\n", secretmsg);

  free(secretchar);
  secretchar = NULL;
  free(secretmsg);
  secretmsg = NULL;
  free(pixels);
  pixels = NULL; 
  return 0;
}