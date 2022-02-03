/*
 * dynamic_snackbar.c
 * Author: Audrey Yang
 * Date: 2/1/2022
 * Like a vending machine, except you can add new snacks to the list of snacks,
 * and you can print out the current list of snacks. Will also not steal your 
 * quarters. 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct snack {
  char name[32];
  float cost;
  int quantity;
};

int main() {
  int numberOfSnackies; // number of snacks
  struct snack *snackies; // array that holds all the snacks
  struct snack addSnackie; // the snack that will be added to the snack array

  // snack detail information
  char snackieName[32];
  float snackieCost;
  int snackieQuantity;
  
  // get the total number of snacks
  printf("Enter a number of snacks: ");
  scanf("%d", &numberOfSnackies);  
  
  // set number of snacks
  snackies = malloc(sizeof(struct snack) * numberOfSnackies);
  // check if malloc worked properly. if not: error message  
  if(snackies == NULL){
    printf("Cannot make the snackbar. "
           "Did you enter an APPROPRIATE amount of snacks?\n");
    exit(1);
  }

  // add the snacks  
  for(int i = 0; i < numberOfSnackies; i++){
    // printf("Snack #%d\n", i); 
    // get the snack details
    printf("Enter a name: ");
    scanf("%32s", snackieName);
    printf("Enter a cost: ");
    scanf("%f", &snackieCost);
    printf("Enter a quantity: ");
    scanf("%d", &snackieQuantity);
  
    // create the snack to add
    strcpy(addSnackie.name, snackieName);
    addSnackie.cost = snackieCost;
    addSnackie.quantity = snackieQuantity;
     
    // add the snack to the snack array
    snackies[i] = addSnackie;
  }
  
  // print out all the snacks
  printf("\nWelcome to the Dynamic Snack Bar!\n\n");
  for(int i = 0; i < numberOfSnackies; i++){
      printf("%d) %s\t cost: $%.2f\t quantity: %d\n", 
              i,
              snackies[i].name,
              snackies[i].cost, 
              snackies[i].quantity);
   }

  free(snackies);
  snackies = NULL;

  return 0;
}
