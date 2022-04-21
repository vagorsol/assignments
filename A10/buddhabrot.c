/**
 * buddahbrot.c
 * Author: Audrey Yang
 * Date: 4/21/2022
 * Outputs an image of the Buddhabrot
 * using multithreading.
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <math.h>
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
  int *mandelship;
  int *mandelcounts;
  struct ppm_pixel *pixels;
  struct ppm_pixel *palette;
};

pthread_mutex_t mutex;
pthread_barrier_t barrier; 
static int max_count = 0;

void* buddahbot(void* args){
  struct thread_args *vals = args;
  struct ppm_pixel color;

  printf("Thread %lu) sub-image block: cols(%d, %d) to rows (%d, %d)\n",
          pthread_self(), vals->rowBot, vals->rowTop, vals->colBot, vals->colTop);
  // determine mandelbrot set membership
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
        // set membership at (row, col) to false
        pthread_mutex_lock(&mutex);
        vals->mandelship[row * vals->size + col] = 0;
        pthread_mutex_unlock(&mutex);

      } else{
        // set membershi at (row, col) to true
        pthread_mutex_lock(&mutex);
        vals->mandelship[row * vals->size + col] = 1;
        pthread_mutex_unlock(&mutex);
      }
    }
  }
 
  // compute visited counts
  for(int row = vals->rowBot; row < vals->rowTop; row++){
    for(int col = vals->colBot; col < vals->colTop; col++){
      // this might should be if mandelship == 0
      if(vals->mandelship[row * vals->size + col] == 0){
        float xfrac = col / vals->size;
        float yfrac = row / vals->size;
        float x0 = vals->xmin + xfrac * (vals->xmax - vals->xmin);
        float y0 = vals->ymin + yfrac * (vals->ymax - vals->ymin);

        float x = 0.0;
        float y = 0.0;
        int max = 0;

        while(x*x + y*y <= 4){
          // TODO: values are only 0
          float xtemp = x*x - y*y + x0;
          y = 2*x*y + y0;
          x = xtemp; 

          int yrow = round(vals->size * ((y - vals->ymin)/(vals->ymax - vals->ymin)));
          int xcol = round(vals->size * ((x - vals->xmin)/(vals->xmax - vals->xmin)));

          if(yrow < 0 || yrow >= vals->size) continue;
          if(xcol < 0 || xcol >= vals->size) continue;
          printf("yrow: %d xcol: %d\n", yrow, xcol);
          pthread_mutex_lock(&mutex);
          vals->mandelcounts[yrow * vals->size + xcol] += 1;
          max_count += 1;
          pthread_mutex_unlock(&mutex);
        } 
      }
    }
  }
  
  // WAIT (pthread_barrier_t)
  pthread_barrier_wait(&barrier);

  // compute colors
  float gamma = 0.681;
  float factor = 1.0 / gamma; 

  for(int row = vals->rowBot; row < vals->rowTop; row++){
    for(int col = vals->colBot; col < vals->colTop; col++){
      float value = 0;

      if(vals->mandelcounts[row * vals->size + col] > 0){
        printf("Mandelcount at (%d, %d) is: %d\n", row, col, vals->mandelcounts[row * vals->size + col]);
        value = log(vals->mandelcounts[row * vals->size + col]) / log(max_count);
        value = pow(value, factor);
      }

      pthread_mutex_lock(&mutex);
      // printf("Color at (%d, %d) is: %f\n", row, col, value * 255);
      vals->pixels[row * vals->size + col].red = value * 255;
      vals->pixels[row * vals->size + col].green = value * 255;
      vals->pixels[row * vals->size + col].blue = value * 255;
      pthread_mutex_unlock(&mutex);
    }
  }

  printf("Thread %lu) finished\n", pthread_self());
  return (void *)0;
}

int main(int argc, char* argv[]) {
  int size = 480;
  float xmin = -2.5;
  float xmax = 1;
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
  printf("Generating buddhabrot with size %dx%d\n", size, size);
  printf("  Num processes = %d\n", numProcesses);
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
  int *mandelship; // is the point in the mandelbrot
  int *mandelcounts; // amount of visits

  palette = malloc(sizeof(struct ppm_pixel) * maxIterations);
  if(palette == NULL){
    printf("ERROR: unable to generate a palette.\n");
    exit(1);
  }
  mandelship = malloc(sizeof(int) * image_size);
  if(mandelship == NULL){
    printf("ERROR: unable to allocate memory for an array.\n");
    exit(1);
  }
  mandelcounts = calloc(image_size, sizeof(int));
  if(mandelcounts == NULL){
    printf("ERROR: unable to allocate memory for an array.\n");
    exit(1);
  }

  // generate palette
  for(int i = 0; i < maxIterations; i++){
    palette[i].colors[0] = rand() % 255;
    palette[i].colors[1] = rand() % 255;
    palette[i].colors[2] = rand() % 255;
  }

  pthread_t threads[4];
  pthread_barrier_init(&barrier, NULL, 4);
  // start time
  struct timeval tstart, tend; 
  gettimeofday(&tstart, NULL);

  int sizediv = size / 2;
  struct thread_args args[4];

  pthread_mutex_init(&mutex, NULL);

  // compute image 
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
    args[i].mandelship = mandelship;
    args[i].mandelcounts = mandelcounts;
    switch(i){
      case 0: args[i].colBot = 0; args[i].colTop = sizediv; args[i].rowBot = 0; args[i].rowTop = sizediv; break;
      case 1: args[i].colBot = sizediv; args[i].colTop = size; args[i].rowBot = 0; args[i].rowTop = sizediv; break;
      case 2: args[i].colBot = 0; args[i].colTop = sizediv; args[i].rowBot = sizediv; args[i].rowTop = size; break;
      case 3: args[i].colBot = sizediv; args[i].colTop = size; args[i].rowBot = sizediv; args[i].rowTop = size; break;
    }
    pthread_create(&threads[i], NULL, buddahbot, &args[i]);
  }

  for(int i = 0; i < 4; i++){
    pthread_join(threads[i], NULL);
  }

  // end time
  gettimeofday(&tend, NULL);
  double timer = tend.tv_sec - tstart.tv_sec + (tend.tv_usec + tstart.tv_usec) / 1.e6;

  printf("Computed buddhabrot set (%dx%d) in %.6f seconds\n", size, size, timer);

  // make the file name
  char *outFname = NULL;
  int timestamp = time(0); 
  int outsize = sizeof("buddhabrot") + sizeof(size) + 17;
  outFname = calloc(outsize + 2, sizeof(char)); 
  if(outFname == NULL){
    printf("ERROR: unable to write a file name!\n");
    exit(1);
  }

  sprintf(outFname,"%s%d%s%d%s", "buddhabrot-", size, "-", timestamp, ".ppm");

  // write file
  write_ppm(outFname, pixels, size, size);

  printf("Writing file: %s\n", outFname);

  free(outFname);
  outFname = NULL;
  free(pixels);
  pixels = NULL;
  free(palette);
  palette = NULL;
  free(mandelcounts);
  mandelcounts = NULL;
  free(mandelship);
  mandelship = NULL;
  pthread_barrier_destroy(&barrier);
  pthread_mutex_destroy(&mutex);

  return 0;
}
