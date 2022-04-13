/**
 * multi_mandelbrot.c
 * Author: Audrey Yang
 * Date: 4/7/2022
 * Creats fractals based on the mandelbrot set
 * using forks and child processes.
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
#include "read_ppm.h"

int main(int argc, char* argv[]) {
  int size = 2000;
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
  int shmid = shmget(IPC_PRIVATE, sizeof(struct ppm_pixel) * image_size, 0644 | IPC_CREAT);
  if(shmid == -1){
    perror("ERROR: cannot initialize shared memory\n");
    exit(1);
  }

  struct ppm_pixel *pixels = shmat(shmid, NULL, 0);
  if(pixels == (void*) - 1){
    perror("Error: cannot access shared memory\n");
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
 
  int colBot, rowBot, colTop, rowTop; 
  int sizediv = size / 2;

  for(int i = 0; i < 4; i++){
    int pid = fork();
    if(pid == 0){
      // set up the parameters
      switch(i){
        case 0: colBot = 0; colTop = sizediv; rowBot = 0; rowTop = sizediv; break;
        case 1: colBot = sizediv; colTop = size; rowBot = 0; rowTop = sizediv; break;
        case 2: colBot = 0; colTop = sizediv; rowBot = sizediv; rowTop = size; break;
        case 3: colBot = sizediv; colTop = size; rowBot = sizediv; rowTop = size; break;
      }
      int ppid = getpid();
      printf("%d) Sub-image block: cols (%d, %d) to rows (%d, %d)\n", 
                  ppid,
                  colBot,
                  colTop,
                  rowBot,
                  rowTop);
      // compute image
      for(int row = rowBot; row < rowTop; row++){
        for(int col = colBot; col < colTop; col++){
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
      exit(0);
    } else {
      printf("Launched child process: %d\n", pid);
    }
  }

  for(int i = 0; i < 4; i++){
    int status;
    int pid = wait(&status);
    printf("Child process complete: %d\n", pid);
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

  // note to self: ask about "still reachable" memory in leaks check 
  // also the "2 allocs 1 free" even though at end it says "6 allocs 6 frees"
  free(outFname);
  outFname = NULL;
  free(palette);
  palette = NULL;

  if(shmdt(pixels) == -1){
    perror("ERROR: cannot detach from shared memory\n");
    exit(1);
  }

  if(shmctl(shmid, IPC_RMID, 0) == -1){
    perror("ERRORL cannot remove shared memory\n");
    exit(1);
  }
  return 0;
}
