/*
 * read_ppm.c
 * Author: Audrey Yang
 * Date: 2/21/2022
 * An implementation of the read_ppm() and write_ppm() functions from read_ppm.h
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "read_ppm.h"

/*
 * Reads a PPM file in raw (binary) format and returns an array of all the 
 * RGB values of each pixel
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

/*
 * Given an array of pixels, make a new PPM file where all the pixels are 
 * color shifted ("glitched")
 * param: filename (file write out to), pxs (array of pixels),
 * w (width of the file), h (height of the file)
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
    
  srand(time(0)); 
  // color shift
  int indx;
  for(int i = 0; i < h; i++){
    for(int j = 0; j < w; j++){
      indx = i * w + j;
      unsigned int masklow = 0x08;
      unsigned int maskhigh = 0x80;
      unsigned int maskmid = 0x3C;

      // trying to make glitch lines by doing funky things
      if(j % 2 == 0 || j % 3 == 0){
        // makes things pink  
        pxs[indx].red = ~(pxs[indx].red &  masklow);
        pxs[indx].blue = (pxs[indx].green ^ maskhigh);
      } else if (i % 5 == 0 && j % 2 == 0){
        pxs[indx].red = ~pxs[indx].red;
        pxs[indx].green = (pxs[indx].green & maskmid); 
      } else if(i % 3 == 0){
        pxs[indx].green = (pxs[indx].red | maskmid);
      } else{
        // this combo makes it blue. i like blue.        
        pxs[indx].red = (pxs[indx].red & masklow);
        pxs[indx].blue = (pxs[indx].blue ^ maskhigh);
      }
      pxs[indx].red = pxs[indx].red << (rand() % 2);
      pxs[indx].green = pxs[indx].green << (rand() % 2);
      pxs[indx].blue = pxs[indx].blue << (rand() % 2);
      
    }
  }
 
  fwrite(pxs, sizeof(struct ppm_pixel), w * h, infile);
  fclose(infile);
  free(arrsize);
  arrsize = NULL;
}
