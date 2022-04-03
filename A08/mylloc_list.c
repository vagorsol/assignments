#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// chunk size is 161
struct chunk {
  int size;
  int used; 
  struct chunk *next;
};
struct chunk *flist = NULL;

void *malloc (size_t size) {
  if (size == 0){
    return NULL;
  }

  struct chunk *next = flist; 
  struct chunk *prev = NULL;

  while(next != NULL){
    if(next->size >= size){
      next->used = size; 
      if(prev != NULL){
        prev->next = next->next;
      } else {
        flist = next->next;
      }
      return (void *)(next + 1);
    } else {
      prev = next;
      next = next ->next;
    }
  }

  void *memory = sbrk(size + sizeof(struct chunk));
  if(memory == (void *)-1){
    return NULL;
  } else {
    struct chunk *cnk = (struct chunk*) memory;
    cnk->size = size;
    cnk->used = size;
    return (void *)(cnk + 1);
  }

}

void free(void *memory) {
  if(memory != NULL){
    struct chunk *cnk = (struct chunk*)((struct chunk*)memory - 1);
    cnk->next = flist;
    cnk->used = 0;
    flist = cnk;
  }
  return;
}

/**
 * Given a list of data, outputs:
 * total number of free chunks allocated, 
 * total number of in-use chunks allocated
 * largest, smallest, and average unused memory across all used chunks
 * total unused memory across all used chunks
 * largest, smallest, and average size of all free chunks
 */
void fragstats(void* buffers[], int len) {
  // get size of element with: 
  int totalBlocks = 0;
  int totalUsed = 0;
  int internTotal = 0, internSmall = 0, internLarge = 0, internCount = 0; // used
  int externTotal = 0, externSmall = 0, externLarge = 0, externCount = 0; // freed
  float internAvg, externAvg;

  for(int i = 0; i < len; i++){
    if(buffers[i] != NULL){
      struct chunk *cnk = (struct chunk*) ((struct chunk*) buffers[i] - 1);

      totalBlocks = totalBlocks + cnk->size; 
      totalUsed = totalUsed + cnk->used; 

      // used memory
      if(cnk->used < cnk->size){
        int unusedSize = cnk->size - cnk->used;
        if(unusedSize < internSmall || (internSmall == 0 && unusedSize != 0)){
          internSmall = unusedSize;
        }
        if(cnk->used > internLarge){
          internLarge = unusedSize;
        }
        internTotal = internTotal + unusedSize;
        internCount++;
      }
    }
  }

  // int casting and divide by 1024 because units
  int totalFree = (int) ((totalBlocks - totalUsed) / 1024);
  totalBlocks = (int) (totalBlocks / 1024);
  totalUsed = (int) (totalUsed / 1024);

  // freed memory
  struct chunk *next = flist; 
  while(next != NULL){
    if(next->size < externSmall || (externSmall == 0 && next->size != 0)){
      externSmall = next->size;
    }
    if(next->size > externLarge){
      externLarge = next->size;
    }
    externTotal = externTotal + next->size;
    externCount++;
    next = next ->next;
  }

  externAvg = (float) externTotal / externCount;
  internAvg = (float) internTotal / internCount; 

  printf("Total Blocks: %d\t Free:  %d \t Used: %d\n", 
          totalBlocks,
          totalFree,
          totalUsed);
  // unused memory stats
  printf("Internal Unused\t\t Total: %d\t Average: %.1f\t Smallest: %d\t Largest: %d\t\n",
          internTotal,
          internAvg,
          internSmall,
          internLarge);
  // free chunks stats
    printf("External Unused\t\t Total: %d\t Average: %.1f\t Smallest: %d\t Largest: %d\t\n",
          externTotal,
          externAvg,
          externSmall,
          externLarge);
}