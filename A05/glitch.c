/* 
 * glitch.c
 * Author: Audrey Yang
 * Date: 2/21/2022
 * Given a (valid) PPM file, "glitch" it and save it as a new file
 */

#include <stdio.h>
#include <stdlib.h>
#include "read_ppm.h"
#include <string.h>

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

  // making the new file name
  char *outFname = NULL; 
  int size = strlen(argv[1]);
  outFname = malloc(sizeof(char) * (size + 12));
  if(outFname == NULL){
    printf("Error! malloc failed! Unable to name output file.\n");
    exit(1);
  }
  strcpy(outFname, argv[1]);
  outFname = strtok(outFname, "."); // removes the ".ppm" from the new filename
  strncat(outFname, "-glitch.ppm", size + 12);
 
  write_ppm(outFname, pixels, width, height);

  printf("Writing file %s\n", outFname);

  free(pixels);
  free(outFname);
  pixels = NULL;
  outFname = NULL;
  return 0;
}
