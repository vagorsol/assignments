//---------------------------------------------------------------------
// sorted_snackbar.c 
// CS223 - Spring 2022
// Ask the user for a list of snacks and store them in alphabetical order
// Name: Audrey Yang
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct snack {
  char name[32];
  int quantity;
  float cost;
  struct snack* next;
};

// Insert a new node to a list (implemented as a linked list). 
// The new node should store the given properties
// Param snacks: the first item in the list (NULL if empty)
// Param name: the snack name (max length is 32 characters)
// Param quantity: the snack quantity
// Param cost: the snack cost
// Returns the first item in the list
struct snack* insert_sorted(struct snack* snacks, 
  const char* name, int quantity, float cost) {
  // make the new node to add
  struct snack* snackNode = malloc(sizeof(struct snack));
  if(snackNode == NULL){
    printf("ERROR: Out of space!\n");
    exit(1);
  }  

  // add things to the new node
  strcpy(snackNode->name, name);
  snackNode->quantity = quantity;
  snackNode->cost = cost; 

  // for inserting nodes
  int insertedQ = 0; // bool value to keep track of if its inserted yet
  struct snack* prevNode; 
  struct snack* currNode;
  struct snack* tempPtr; 

  // initialize nodes as the start
  prevNode = snacks;
  currNode = snacks;
  tempPtr = snacks;
  
  if(snacks == NULL){
    // for the first snack, just make it the head
    snackNode->next = NULL;
    insertedQ = 2;  
  } else {
    // otherwise: insert sort 
  
    while(insertedQ == 0){
      if(currNode == NULL){
        // if hit end, just plug it in
        prevNode->next = snackNode; 
        snackNode->next = NULL;
        insertedQ = 1;
      } else if(strcmp(snackNode->name, currNode->name) < 0){
        if(currNode != snacks){
          // if the snackNode is alphabetically before currNode, insert there
          tempPtr = currNode;
          prevNode->next = snackNode;
          snackNode->next = tempPtr;
          insertedQ = 1;
        } else { 
          // specific case for if it's replacing the starting node
          snackNode->next = snacks; 
          insertedQ = 2;
        }
      } else{
        // move along the list
        prevNode = currNode;
        currNode = currNode->next;
      }
    } 
  }
  // free things ft. doing weird things to not double free things 
  tempPtr = NULL;   
  currNode = NULL;
  prevNode = NULL;
  if(insertedQ == 1){
    return snacks;
  } else if (insertedQ == 2){
    return snackNode;
  } else{
    return NULL;
  }
}

// Delete (e.g. free) all nodes in the given list of snacks
// Param snacks: the first node in the list (NULL if empty)
void clear(struct snack* snacks) {
  if(snacks != NULL){
    struct snack *next;
    next;
    next = snacks->next;
    free(snacks);
    snacks = NULL;
    clear(next);
  }
}

int main() {
  int numberOfSnacks; // number of snacks
  struct snack *snackStart; // first snack in the list
  
  // initliaze the first snack to nothing
  snackStart = NULL;  

  // snack details
  char snackName[32];
  int snackQuantity;
  float snackCost;  

  printf("Enter a number of snacks: ");
  scanf(" %d", &numberOfSnacks);

  for(int i = 0; i < numberOfSnacks; i++){
    // get snack info
    printf("Enter a name: ");
    scanf("%32s", snackName);
    printf("Enter a cost: ");
    scanf("%f", &snackCost);
    printf("Enter a quantity: ");
    scanf(" %d", &snackQuantity);

    // store and sort snack
    snackStart = insert_sorted(snackStart, snackName, snackQuantity, snackCost);
  }
  
  printf("\nWelcome to THE SORT SNHCNAK BAR!\n\n");
  
  // probably could have just used snackStart
  // but wanted to be safe with messing with values  
  struct snack* tempPtr;
  tempPtr;
  tempPtr = snackStart;

  // print out all the snacks
  for(int i = 0; i < numberOfSnacks; i++){
    printf("%d) %s\t cost: $%.2f\t quantity: %d\n",
            i,
            tempPtr->name,
            tempPtr->cost,
            tempPtr->quantity); 
    tempPtr = tempPtr->next;
  }
  
  clear(snackStart);
  tempPtr = NULL; 
  return 0;
}
