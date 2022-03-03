/*
 * read_ppm.c
 * Author: Audrey Yang
 * Date: 2/23/2022
 * An implementation of the read_ppm() and write_ppm() functions from read_ppm.h
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "read_ppm.h"

/*
 * Reads a PPM file in binary format and returns an array with all the RGB
 * values of all the pixels
 */
struct ppm_pixel* read_ppm(const char* filename, int* w, int* h) {
  FILE *infile;
  // check that file can be opened
  infile = fopen(filename, "rb");
  if(infile == NULL){
    return NULL;
  } 
  // read header
  int count = 0;
  // char filetype[3];
  char c[100]; 
  while(count < 3){
    fgets(c, 100, infile); 
    if(c[0] != '#'){
      // getting the dimensions of the picture
      if(count == 1){
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
   
  fread(pixels, sizeof(struct ppm_pixel), *w * *h, infile);

  fclose(infile); 
  return pixels;
}

/*
 * Given an array of pixels, write a new PPM file with the given pixels
 */
extern void write_ppm(const char* filename, struct ppm_pixel* pxs, int w, int h) {
  FILE *infile;
  infile = fopen(filename, "wb");
  if(infile == NULL){
    printf("Error: unable to open outfile.\n");
    exit(1);
  }

  // header information
  char filetype[] = "P6\n";
  char *arrsize;
  arrsize = malloc(sizeof(int) * 5);
  if(arrsize == NULL){
    printf("ERROR: unable to write header.\n");
    exit(1);
  }
  sprintf(arrsize,"%d %d\n", w, h);
  char maxpixval[] = "225\n";
  fwrite(filetype, sizeof(filetype) - 1, 1, infile);
  fwrite(arrsize, sizeof(arrsize), 1, infile);
  fwrite(maxpixval, sizeof(maxpixval) - 1, 1, infile); 
  
  // add the pixels
  fwrite(pxs, sizeof(struct ppm_pixel), w * h, infile);
  fclose(infile);
  free(arrsize);
  arrsize = NULL;
}