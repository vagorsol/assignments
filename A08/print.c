/**
 * print.c
 * Author: Audrey Yang
 * Date: 3/29/2022
 * Implementation of basic memory allocation demostration
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include "rand.h"

int main (int argc, char* argv[]) {
  void* top1 = sbrk(0);
  printf("The initial top of the head is %p\n", top1);
  void* top2 = sbrk(0);
  printf("The initial top of the head is %p\n", top2);
  int sum = top2 - top1;
  printf("Increased by %d (0x%3x) bytes\n", sum, sum);
  return 0;
}
