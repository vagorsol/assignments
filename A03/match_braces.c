//---------------------------------------------------------------------
// match_braces.c 
// CS223 - Spring 2022
// Identify matched braces from a given file
// Name: Audrey Yang
//
#include <stdio.h>
#include <stdlib.h>

struct node {
  char sym;
  int linenum;
  int colnum;
  struct node* next;
};

// Push a new node to a stack (implemented as a linked list). 
// The new node should store the given symbol, line number, and column number
// Param sym: a character symbol, '{' for this program
// Param line: the line number of the symbol
// Param line: the column number of the symbol
// Param top: the top node of the stack (NULL if empty)
// Returns the new top of the stack
struct node* push(char sym, int line, int col, struct node* top) {
  // make the new node here
  struct node* newNode; 
  newNode = malloc(sizeof(struct node));
  if(newNode == NULL){
    printf("ERROR: OUt of space! Couldn't make a new node :(\n");
    exit(1);
  }

  // add the elements to the new node
  newNode->sym = sym; // check if I actually need to strcpy this
  newNode->linenum = line;
  newNode->colnum = col;
  
  // if it's the first entry, just do this
  if(top == NULL){
    newNode->next = NULL;
  } else{
    // replace the starting node (push it)
    newNode->next = top;
  }
  return newNode;
}

// Pop the top node from a stack (implemented as a linked list) and frees it
// Param top: the top node of the current stack (NULL if empty)
// Returns the new top of the stack, NULL if the stack is (now) empty
struct node* pop(struct node* top) {
  if(top != NULL){
  struct node* newHead;
  newHead = top->next;
  // free the current head
  free(top);
  top = NULL;
  // return the new head
  return newHead;
  } else{
    // if the stack is empty to begin with, return NULL
    return NULL;
  }
}

// Delete (e.g. free) all nodes in the given stack
// Param top: the top node of the stack (NULL if empty)
void clear(struct node* top) {
  struct node* nextNode;
  nextNode = top;
  if(top != NULL){
    nextNode = top->next;
    free(top);
    top = NULL;
    clear(nextNode);
  }
}

// Print all nodes in the given stack (from top to bottom)
// Param top: the top node of the stack (NULL if empty)
void print(struct node* top) {
  struct node* nextNode;
  // nextNode;
  if(top != NULL){
    printf("Char: %c\tLine: %d\tColumn: %d\n", top->sym, top->linenum, top->colnum);
    nextNode = top->next;
    print(nextNode);
  }
}

int main(int argc, char* argv[]) {
  FILE *infile;
  
  // open and read the file  
  infile = fopen(argv[1], "r"); 
  if(infile == NULL){
    printf("Cannot open the file: %s\n", argv[1]);
    exit(1);
  }
  
  // the actual program here sigh.
  int column, line, end;
  struct node* top; // the starting node
  top = NULL;
  column = 0;
  line = 1;
  int c; // char read in 
  c = 0; // initializing c to Some Value so the while bool condition works

  // read things in
  while(c != EOF){
    c = fgetc(infile);
    if(c == '\n'){
      // when you reach the end of a line, update line & reset column values
      line = line + 1;
      column = 0;
    } 
    if(c == '{'){
      // push 
      top = push(c, line, column, top); 
      // printf("Pushing!\n");
      // print(top);
    } 
    if(c == '}'){
      // print(top);
      // if pop is valid, print out thing. 
      if(top != NULL){
        printf("Found matching braces: (%d, %d) -> (%d, %d)\n",
                top->linenum,
                top->colnum,
                line,
                column);
        top = pop(top);
      } else{
        // if pop returns null, then you have a brace error
        printf("Unmatched brace on Line %d and Column %d\n", line, column);
      }
    }
    column++;  
  }
  
  struct node* check; 
  check = top;
  // check if there is anything left in the stack, if so it's an error! hooray!
  // keep repeating until there is nothing left in the stack
  while(check != NULL){
    printf("Unmatched brace on Line %d and Column %d\n", 
            check->linenum, 
            check->colnum);
    check = check->next;
  }

  clear(top); 
  return 0;
}
