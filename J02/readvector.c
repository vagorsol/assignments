#include <stdio.h>
#include <stdlib.h>

// readvector opens and reads the given file
// The size of the vector should be stored in the parameter size
// The values should be returned in a flat float array
float* readvector(const char* filename, int *size) {
  FILE *infile;
  infile = fopen(filename, "r");
  if(infile == NULL){
    return NULL;
  }
  char c[32];
  fgets(c, 32, infile);
  *size = atoi(c);
  float *ret;
  ret = malloc(sizeof(float) * *size);
  if(ret == NULL){
    return NULL;
  }

  for(int i = 0; i < *size; i++){
    fgets(c, 32, infile);
    ret[i] = atof(c); 
  }
  fclose(infile); 
  return ret;
}

int main(int argc, char** argv) {
  if (argc != 2) {
    printf("usage: readvector <filename>\n");
    exit(0);
  }

  int size = 0;
  float* vector = readvector(argv[1], &size);
  if(vector == NULL){
    printf("ERROR: unable to create a vector\n");
    exit(1);
  }
  // printf("Size of vector: %d\n", size);
  for(int i = 0; i < size; i++){
    printf("%f\n", vector[i]);
  }

  free(vector);
  return 0;
}

