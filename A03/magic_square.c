//---------------------------------------------------------------------
// magic_square.c 
// CS223 - Spring 2022
// Identify whether a matrix is a magic square
// Name: Audrey Yang
//
#include <stdio.h>
#include <stdlib.h>

// calculates the sum along the rows, columns, or diagonal based on
// dir, which tells whether it will be finding the sum along the row (1),
// the column (2), or the diagonal (3)
// row: start row coor, col: start column coor
// len: length (dimensions) of the array
int lineSum(int row, int col, int len, int dir, int **arr){
  int ret;
  ret = 0;
  // sum along row
  if(dir == 1){
    for(int i = 0; i < len; i++){
      ret = ret + arr[row][col + i];
    }
  } 

  // sum along column
  if(dir == 2){
    for(int i = 0; i < len; i++){
      ret = ret + arr[row + i][col];
    }
  }

  // sum along diagonal
  if(dir == 3 && row != len - 1){
    for(int i = 0; i < len; i++){
      ret = ret + arr[row + i][col + i];
    }
  }
  // accounting for the other diagonal case bc i already committed to having 
  // all the summing in one function
  if(dir == 3 && row == len - 1){
    for(int i =0; i < len; i++){
      ret = ret + arr[row - i][col -i];
    }
  }
  return ret; 
}

int main() {
  int columns;
  int rows;
  int **matrix;
  int magicConstant;
  int tempMagicConstant;
  int isMagicSquare; // bool value  
  int innit; 
  
  // get the size of the matrix
  scanf(" %d", &rows);
  scanf(" %d", &columns);
 
  // allocate space for the matrix 
  matrix = malloc(sizeof(int *) * rows);
  if(matrix == NULL){
    printf("Unable to create the matrix.\n");
    exit(1);
  }
  // allocate space for the arrays in the array
  for(int i = 0; i < rows; i++){
    matrix[i] = malloc(sizeof(int) * columns);
    if(matrix[i] == NULL){
      printf("Unable to create column in row %d\n", i);
      exit(1);
    }
  }

  // get the matrix in
  for(int i = 0; i < rows; i++){
    for(int j = 0; j < columns; j++){
      scanf(" %d", &innit);
      matrix[i][j] = innit;
    }
  }

  // print out the matrix
  for(int i = 0; i < rows; i++){
    for(int j = 0; j < columns; j++){
      printf("%d ", matrix[i][j]); // may need to derefence this idk
      if(j + 1 == columns){
        printf("\n");
      }
    }
  }

  // check if the matrix is a magic number 
  if(rows != columns){
    // if the matrix isn't square then it just isn't a magic square
    isMagicSquare = 0; 
  } else{
    // check the rows
    for(int i = 0; i < rows; i++){
      tempMagicConstant = lineSum(i,0, rows, 1, matrix);
      // printf("CURRENT CONST (ROW): %d\t i: %d\n", tempMagicConstant, i); 
      if(i == 0){
        // for the first one just set it
        // because just setting it as 0 doesn't work since it could be the 
        // magic constant :/
        magicConstant = tempMagicConstant;
      } else if(magicConstant != tempMagicConstant){
        // if the sum of the line isn't equal to the magic constant, 
        // it's not a magic square
        isMagicSquare = 0;
        break;
      }
    }
    // check the columns
    for(int i = 0; i < columns; i++){
        tempMagicConstant = lineSum(0, i, columns, 2, matrix);
        // printf("CURRENT CONST (COLUMN): %d\t i: %d\n", tempMagicConstant, i); 
      if(i == 0){
        if(magicConstant != tempMagicConstant){
          isMagicSquare = 0;
          break;
        }
      }
    }
    // check the diagonals
    tempMagicConstant = lineSum(0, 0, rows, 3, matrix);
    if(magicConstant != tempMagicConstant){
      isMagicSquare = 0;
    }
    tempMagicConstant = lineSum(rows - 1, columns - 1, rows, 3, matrix);
    // sort of redundant but i don't know how to just make the if statment
    // break out of the entire else statement
    if(magicConstant != tempMagicConstant){
       isMagicSquare = 0;
    }
  }

  // print out  the results
  if(isMagicSquare != 0){
    printf("M is a magic square (magic constant = %d)\n", magicConstant);
  } else{
    printf("M is NOT a magic square!\n");
  }

  // deallocate the matrix
  // i am just Assuming that i need to deallocate the inner arrays
  for(int i = 0; i < rows; i++){
    free(matrix[i]);
    matrix[i] = NULL;   
  }
  free(matrix);
  matrix = NULL;
  return 0;
}

