#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "read_ppm.h"

// Feel free to change the function signature if you prefer to implement an 
// array of arrays
struct ppm_pixel* read_ppm(const char* filename, int* w, int* h) {
  FILE *infile;
  // check that file can be opened
  infile = fopen(filename, "rb");
  if(infile == NULL){
    return NULL;
  }
 
  // read header
  int count = 0;
  char c[100]; // char read in, should be large enough if not well. 
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
