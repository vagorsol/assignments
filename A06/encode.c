/*
 * encode.c
 * Author: Audrey Yang
 * Date: 3/3/2022
 * Encodes a message hidden in a PPM file.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
  maxchar = (width * height * 3) / 8;
  printf("Max number of characters in the image: %d\n", maxchar - 1); 
  char *msg;
  msg = calloc(maxchar, sizeof(char));
  printf("Enter a phrase: ");
  fgets(msg, maxchar, stdin); // trims off any excess values

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
  
  int *binarr = NULL; // stores the converted binary values
  binarr = calloc(maxchar * 8, sizeof(int));
  if(binarr == NULL){
    printf("ERROR: unable to wrtie secret message!\n");
    exit(1);
  }

  // convert secret msg to ascii
  int msglen = strlen(msg);
  int itt = 8;
  // not getting one letter across
  for(int i = 0; i < msglen; i++){
    int asciid = msg[i]; // get the int value
    if(&msg[i] == NULL){
      break;
    }
    int remainder = 0;
    // convert to binary
    while(asciid != 0){
      itt--;
      remainder = asciid % 2;
      binarr[itt] = remainder;
      asciid = asciid / 2;
    }
    itt = (itt + (8 * 2)) - 1;
  }

  int indx; // 2d array indexing
  itt = 0; // indexing along binarr

  // take each least significant bit from each color value
  // then change it as needed to create the secret message
  for(int i = 0; i < height; i++){
    for(int j = 0; j < width; j++){
      indx = i * width + j; 
      for(int k = 0; k < 3; k++){
        if(binarr[itt] == 0){
          pixels[indx].colors[k] = pixels[indx].colors[k] & ~1;
        } else{
          pixels[indx].colors[k] = pixels[indx].colors[k] | 1;
        }
        itt++;
      }
    }
  }

  write_ppm(outFname, pixels, width, height);

  printf("Writing file %s\n", outFname);

  free(msg);
  msg = NULL;
  free(binarr);
  binarr = NULL;
  free(pixels);
  pixels = NULL;
  free(outFname);
  outFname = NULL;
  return 0;
}