 /*
 * test_ppm.c
 * Author: Audrey Yang
 * Date: 2/14/2022
 * A program that tests the read_ppm() function from read_ppm.c
 * using the file "feep-ascii.ppm"
 */

#include <stdio.h>
#include <stdlib.h>
#include "read_ppm.h"

int main() {
  struct ppm_pixel *pixels;
  int width, height;

  pixels = read_ppm("feep-ascii.ppm", &width, &height);
  if(pixels == NULL){
    printf("ERROR: issue in reading the file.\n");
    exit(1);
  }

  printf("Testing file feep-ascii.ppm: %d %d\n", width, height);
  
  int indx; 
  for(int i = 0; i < height; i++){
    for(int j = 0; j < width; j++){
      indx = i * height + j;
      printf("(%hhu, %hhu, %hhu) ", 
              pixels[indx].red, 
              pixels[indx].green,
              pixels[indx].blue);
    }
    printf("\n");
  }  
  
  free(pixels);
  pixels = NULL; 
  return 0;
}

