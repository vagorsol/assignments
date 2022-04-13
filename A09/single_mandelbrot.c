/**
 * single_mandelbrot.c
 * Author: Audrey Yang
 * Date: 4/7/2022
 * Creats fractals based on the mandelbrot set
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include "read_ppm.h"
#include <string.h>

int main(int argc, char* argv[]) {
  int size = 400; // this value is the one that changes
  float xmin = -2.0;
  float xmax = 0.47;
  float ymin = -1.12;
  float ymax = 1.12;
  int maxIterations = 1000;

  int opt;
  while ((opt = getopt(argc, argv, ":s:l:r:t:b:")) != -1) {
    switch (opt) {
      case 's': size = atoi(optarg); break;
      case 'l': xmin = atof(optarg); break;
      case 'r': xmax = atof(optarg); break;
      case 't': ymax = atof(optarg); break;
      case 'b': ymin = atof(optarg); break;
      case '?': printf("usage: %s -s <size> -l <xmin> -r <xmax> -b <ymin> -t <ymax>\n", argv[0]); break;
    }
  }
  printf("Generating mandelbrot with size %dx%d\n", size, size);
  printf("  X range = [%.4f,%.4f]\n", xmin, xmax);
  printf("  Y range = [%.4f,%.4f]\n", ymin, ymax);

  srand(time(0));

  int image_size = size * size; 
  struct ppm_pixel *pixels; //image to write
  pixels = malloc(sizeof(struct ppm_pixel) * image_size);
  if(pixels == NULL){
    printf("ERROR: unable to allocate memory to make madelbrot.\n");
    exit(1);
  }

  struct ppm_pixel color;
  struct ppm_pixel *palette; 
  
  palette = malloc(sizeof(struct ppm_pixel) * maxIterations);
  if(palette == NULL){
    printf("ERROR: unable to generate a palette.\n");
    exit(1);
  }

  // generate palette
  for(int i = 0; i < maxIterations; i++){
    palette[i].colors[0] = rand() % 255;
    palette[i].colors[1] = rand() % 255;
    palette[i].colors[2] = rand() % 255;
  }

  // start time
  struct timeval tstart, tend; 
  gettimeofday(&tstart, NULL);

  // compute image
  for(int row = 0; row < size; row++){
    for(int col = 0; col < size; col++){
      float xfrac = (float) col / size;
      float yfrac = (float) row / size;
      float x0 = xmin + (xfrac * (float) (xmax - xmin));
      float y0 = ymin + (yfrac * (float) (ymax - ymin));

      float x = 0.0;
      float y = 0.0;
      int itt = 0;

      while((itt < maxIterations) && (x*x + y*y <= 4)){
        float xtemp = x*x - y*y + x0;
        y = 2*x*y + y0;
        x = xtemp; 
        itt++;
        float ret = x*x + y*y; 
      }
      if(itt < maxIterations){
        // escaped - get color
        color.colors[0] = palette[itt].colors[0];
        color.colors[1] = palette[itt].colors[1];
        color.colors[2] = palette[itt].colors[2];
      } else{
        // didn't escape - set color to black
        color.colors[0] = 0; 
        color.colors[1] = 0;
        color.colors[2] = 0; 
      }

      // write color to image at loc
      pixels[row * size + col].colors[0] = color.colors[0];
      pixels[row * size + col].colors[1] = color.colors[1];
      pixels[row * size + col].colors[2] = color.colors[2];
    }
  }

  // end time
  gettimeofday(&tend, NULL);
  double timer = tend.tv_sec - tstart.tv_sec + (tend.tv_usec + tstart.tv_usec) / 1.e6;

  printf("Computed madelbrot set (%dx%d) in %.6f seconds\n", size, size, timer);

  // make the file name
  char *outFname = NULL;
  int timestamp = time(0); 
  int outsize = sizeof("madelbrot") + sizeof(size) + 17;
  outFname = calloc(outsize + 2, sizeof(char)); 
  if(outFname == NULL){
    printf("ERROR: unable to write a file name!\n");
    exit(1);
  }

  sprintf(outFname,"%s%d%s%d%s", "madelbrot-", size, "-", timestamp, ".ppm");

  // write file
  write_ppm(outFname, pixels, size, size);

  printf("Writing file: %s\n", outFname);

  free(outFname);
  outFname = NULL;
  free(pixels);
  pixels = NULL;
  free(palette);
  palette = NULL;

  return 0;
}
