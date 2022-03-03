/*
 * encode.c
 * Author: Audrey Yang
 * Date: 3/3/2022
 * Encodes a message hidden in a PPM file.
 */

#include <stdio.h>
#include <stdlib.h>
#include "read_ppm.h"

int main(int argc, char** argv) {
  struct ppm_pixel *pixels;
  int width, height;
  
  if(argv[1] == NULL){
    printf("No file to open!\n");
    exit(1);
  } 
  
  pixels = read_ppm(argv[1], &width, &height);
  if(pixels == NULL){
    printf("ERROR: Cannot open the file: %s\n", argv[1]);
    exit(1);
  }
  printf("Reading %s with width %d and height %d\n", argv[1], width, height);
  int maxchar;
  maxchar = (width * height * 3) /6;
  printf("Max number of characters in the image: %d\n", maxchar);
  char msg[maxchar];
  printf("Enter a phrase: ");
  scanf(" %s", msg);

  int size = strlen(argv[1]);
  char *outFname = NULL;
  outFname = malloc(sizeof(char) * (size + 13));
  if(outFname == NULL){
    printf("ERROR: unable to name output file.\n");
    exit(1); 
  }
  strcpy(outFname, argv[1]);
  outFname = strtok(outFname, ".");
  strncat(outFname, "-encoded.ppm", size + 13);
  
  // TODO: actually gotta glitch the array here first before i write it 
  write_ppm(outFname, pixels, width, height);

  printf("Writing file %s\n", outFname);
  free(pixels);
  pixels = NULL;
  free(outFname);
  outFname = NULL;
  return 0;
}