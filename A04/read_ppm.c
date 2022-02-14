/*
 * read_ppm.c 
 * Author: Audrey Yang
 * Date: 2/14/2022
 * An implementation of the read_ppm function
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "read_ppm.h"

// A function that reads PPM files stored in ACII format
// param: filename (name of PPM file that's being read),
// w (width of the image in the file), h (height of the image in the file)
// returns: 2D array of pixels, NULL if there's a reading/malloc error
struct ppm_pixel* read_ppm(const char* filename, int* w, int* h) {
  FILE *infile;
  // check that file can be opened
  infile = fopen(filename, "r");
  if(infile == NULL){
    return NULL;
  }
 
  // read header
  int count = 0;
  char c[32]; // char read in
  // also. ask if w/h have to be on the same LINE
  while(count < 3){
    fgets(c, 32, infile); 
    if(c[0] != '#'){
      if(count == 1){
        // get the dimensions of the picture
        *w = atoi(&c[0]);
        *h = atoi(&c[2]);
      } 
      count++;
    }
    // else: if the line is a comment, basically ignore it
  } 

  struct ppm_pixel *pixels;
  // check that space can be allocated for array
  pixels = malloc(sizeof(struct ppm_pixel) * *w * *h);
  if(pixels == NULL){
    return NULL; 
  }

  // read in pixels
  struct ppm_pixel pixelToAdd;
  unsigned char r, g, b;
  for(int i = 0; i < *w; i++){
    for(int j = 0; j < *h; j++){
      fscanf(infile, " %hhu %hhu %hhu", &r, &g, &b);
      pixelToAdd.red = r;
      pixelToAdd.green = g;
      pixelToAdd.blue = b;
      pixels[i * *h + j] = pixelToAdd;
    }
  }  

  fclose(infile); 
  return pixels;
}

