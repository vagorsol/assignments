/*
 * read_ppm.c
 * Author: Audrey Yang
 * Date: 2/21/2022
 * An implementation of the read_ppm() and write_ppm() functions from read_ppm.h
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "read_ppm.h"

// Reads a PPM file in raw (binary) format and returns an array of all the 
// RGB values of each pixel
struct ppm_pixel* read_ppm(const char* filename, int* w, int* h) {
  FILE *infile;
  // check that file can be opened
  infile = fopen(filename, "rb");
  if(infile == NULL){
    return NULL;
  }
 
  // read header
  int count = 0;
  char c[100]; 
  while(count < 3){
    fgets(c, 100, infile); 
    if(c[0] != '#'){
      if(count == 1){
        // get the dimensions of the picture
        *w = atoi(strtok(c," "));
        *h = atoi(strtok(NULL," ")); 
      }  
      count++;
    }
  } 

  struct ppm_pixel *pixels;
  pixels = malloc(sizeof(struct ppm_pixel) * *w * *h);
  if(pixels == NULL){
    return NULL; 
  }
  
  // this read the file as one large block. each 3 pixels is one chunk
  // (what sizeof(struct ppm_pixel) does) and it gets put in the array 
  fread(pixels, sizeof(struct ppm_pixel), *w * *h, infile);

  fclose(infile); 
  return pixels;
}

// TODO: Implement this function
// Feel free to change the function signature if you prefer to implement an 
// array of arrays
extern void write_ppm(const char* filename, struct ppm_pixel* pxs, int w, int h) {

}
