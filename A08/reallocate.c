/**
 * reallocate.c
 * Author: Audrey Yang
 * Date: 3/30/2022
 * Repeatedly malloc and frees 100 bytes of memory
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include "rand.h"

int main (int argc, char* argv[]) {
  void* start = sbrk(0); 

  for(int i = 0; i < 10; i++){
    char* ptr = malloc(100);
    if(ptr == NULL){
      printf("ERROR: not enough memory left.\n");
      exit(1);
    }
    free(ptr);
    // printf("Loop %d\n", i);
  }

  void* endloop = sbrk(0);
  int sum = endloop - start;
  // int sum = (int) ((endloop - start) / 1024);

  printf("Start: %p\n", start);
  printf("End: %p\n", endloop);
  printf("Increased by %d (0x%x) bytes\n", sum, sum);
  
  return 0;
}
