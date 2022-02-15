 /*
 * ascii_image.c
 * Author: Audrey Yang
 * Date: 2/14/2022
 * When given a PPM image, it interprets and displays it as ASCII art
 */

#include <stdio.h>
#include <stdlib.h>
#include "read_ppm.h"

int main(int argc, char** argv) {
  struct ppm_pixel *pixels;
  int width, height;
  
  pixels = read_ppm(argv[1], &width, &height);
  if(pixels == NULL){
    printf("ERROR: Cannot open the file: %s\n", argv[1]);
    exit(1); // til that if i don't have stdlib included, exit doesn't work
  }
  
  printf("Reading %s with width %d and height %d\n", argv[1], width, height);

  int indx;
  float intensity;
  for(int i = 0; i < height; i++){
    for(int j = 0; j < width; j++){
      indx = i * width + j;
      // ascii intesity calculations
      intensity = (pixels[indx].red + pixels[indx].green + pixels[indx].blue);
      intensity = intensity / 3;
      // printf("\nIntensity at %d is: %f\n", indx, intensity);
      // print out the corresponding character for the ascii intensity
      if(intensity >= 0 && intensity <= 25){
        printf("@");
      } else if(intensity >= 26 && intensity <= 50){
        printf("#");
      } else if(intensity >= 51 && intensity <= 75){
        printf("%%");
      } else if(intensity >= 76 && intensity <= 100){
        printf("*");
      } else if(intensity >= 101 && intensity <= 125){
        printf("o");
      } else if(intensity >= 126 && intensity <= 150){
        printf(";");
      } else if(intensity >= 151 && intensity <= 175){
        printf(":");
      } else if(intensity >= 176 && intensity <= 200){
        printf(",");
      } else if(intensity >= 201 && intensity <= 225){
        printf(".");
      } else {
        printf(" "); 
      }
    }
    printf("\n");
  }

  free(pixels);
  pixels = NULL;
  return 0;
}

