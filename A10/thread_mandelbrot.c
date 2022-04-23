/**
 * thread_mandelbrot.c
 * Author: Audrey Yang
 * Date: 4/20/2022
 * Creates fractals based on the mandelbrot set
 * using threads.
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <pthread.h>
#include "read_ppm.h"

struct thread_args{
  int rowBot;
  int rowTop;
  int colBot;
  int colTop;
  float xmin;
  float xmax; 
  float ymin;
  float ymax;
  int size;
  int maxIterations;
  struct ppm_pixel *pixels;
  struct ppm_pixel *palette;
};

pthread_mutex_t mutex; 

void *mandelbot(void* args){
  struct thread_args *vals = args;
  struct ppm_pixel color;

  printf("Thread %lu) sub-image block: cols(%d, %d) to rows (%d, %d)\n",
          pthread_self(), vals->rowBot, vals->rowTop, vals->colBot, vals->colTop);
  // compute image
  for(int row = vals->rowBot; row < vals->rowTop; row++){
    for(int col = vals->colBot; col < vals->colTop; col++){
      float xfrac = (float) col / vals->size;
      float yfrac = (float) row / vals->size;
      float x0 = vals->xmin + (xfrac * (float) (vals->xmax - vals->xmin));
      float y0 = vals->ymin + (yfrac * (float) (vals->ymax - vals->ymin));

      float x = 0.0;
      float y = 0.0;
      int itt = 0;

      while((itt < vals->maxIterations) && (x*x + y*y <= 4)){
        float xtemp = x*x - y*y + x0;
        y = 2*x*y + y0;
        x = xtemp; 
        itt++; 
      }
      if(itt < vals->maxIterations){
        // escaped - get color
        pthread_mutex_lock(&mutex);
        color.colors[0] = vals->palette[itt].colors[0];
        color.colors[1] = vals->palette[itt].colors[1];
        color.colors[2] = vals->palette[itt].colors[2];
        pthread_mutex_unlock(&mutex);
      } else{
        // didn't escape - set color to black
        color.colors[0] = 0; 
        color.colors[1] = 0;
        color.colors[2] = 0; 
      }
      // write color to image at loc
      pthread_mutex_lock(&mutex);
      vals->pixels[row * vals->size + col].colors[0] = color.colors[0];
      vals->pixels[row * vals->size + col].colors[1] = color.colors[1];
      vals->pixels[row * vals->size + col].colors[2] = color.colors[2];
      pthread_mutex_unlock(&mutex);
    }
  }

  printf("Thread %lu) finished\n", pthread_self());
  return (void *)0; 
}

int main(int argc, char* argv[]) {
  int size = 480;
  float xmin = -2.0;
  float xmax = 0.47;
  float ymin = -1.12;
  float ymax = 1.12;
  int maxIterations = 1000;
  int numProcesses = 4;

  int opt;
  while ((opt = getopt(argc, argv, ":s:l:r:t:b:p:")) != -1) {
    switch (opt) {
      case 's': size = atoi(optarg); break;
      case 'l': xmin = atof(optarg); break;
      case 'r': xmax = atof(optarg); break;
      case 't': ymax = atof(optarg); break;
      case 'b': ymin = atof(optarg); break;
      case '?': printf("usage: %s -s <size> -l <xmin> -r <xmax> "
        "-b <ymin> -t <ymax> -p <numProcesses>\n", argv[0]); break;
    }
  }
  printf("Generating mandelbrot with size %dx%d\n", size, size);
  printf("  Num processes = %d\n", numProcesses);
  printf("  X range = [%.4f,%.4f]\n", xmin, xmax);
  printf("  Y range = [%.4f,%.4f]\n", ymin, ymax);

  srand(time(0));

  int image_size = size * size; 
  struct ppm_pixel *pixels; //image to write
  pixels = malloc(sizeof(struct ppm_pixel) * image_size);
  if(pixels == NULL){
    printf("ERROR: unable to allocate memory to make mandelbrot.\n");
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

  pthread_t threads[4];

  // start time
  struct timeval tstart, tend; 
  gettimeofday(&tstart, NULL);

  // compute image 
  int sizediv = size / 2;
  struct thread_args args[4];

  pthread_mutex_init(&mutex, NULL);
  for(int i = 0; i < 4; i++){
    // define params
    args[i].xmin = xmin;
    args[i].xmax = xmax;
    args[i].ymin = ymin;
    args[i].ymax = ymax; 
    args[i].size = size;
    args[i].maxIterations = maxIterations;
    args[i].pixels = pixels;
    args[i].palette = palette;
    switch(i){
      case 0: args[i].colBot = 0; args[i].colTop = sizediv; args[i].rowBot = 0; args[i].rowTop = sizediv; break;
      case 1: args[i].colBot = sizediv; args[i].colTop = size; args[i].rowBot = 0; args[i].rowTop = sizediv; break;
      case 2: args[i].colBot = 0; args[i].colTop = sizediv; args[i].rowBot = sizediv; args[i].rowTop = size; break;
      case 3: args[i].colBot = sizediv; args[i].colTop = size; args[i].rowBot = sizediv; args[i].rowTop = size; break;
    }
    pthread_create(&threads[i], NULL, mandelbot, &args[i]);
  }

  for(int i = 0; i < 4; i++){
    pthread_join(threads[i], NULL);
  }
  // end time
  gettimeofday(&tend, NULL);
  double timer = tend.tv_sec - tstart.tv_sec + (tend.tv_usec + tstart.tv_usec) / 1.e6;

  printf("Computed mandelbrot set (%dx%d) in %.6f seconds\n", size, size, timer);

  // make the file name
  char *outFname = NULL;
  int timestamp = time(0); 
  int outsize = sizeof("mandelbrot") + sizeof(size) + 17;
  outFname = calloc(outsize + 2, sizeof(char)); 
  if(outFname == NULL){
    printf("ERROR: unable to write a file name!\n");
    exit(1);
  }

  sprintf(outFname,"%s%d%s%d%s", "mandelbrot-", size, "-", timestamp, ".ppm");

  // write file
  write_ppm(outFname, pixels, size, size);

  printf("Writing file: %s\n", outFname);

  free(outFname);
  outFname = NULL;
  free(pixels);
  pixels = NULL;
  free(palette);
  palette = NULL;
  pthread_mutex_destroy(&mutex);

  return 0;
}