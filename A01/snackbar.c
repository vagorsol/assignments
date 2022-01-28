/*
 * snackbar.c
 * Author: Audrey Yang
 * Date: 1/27/2022
 * Like a vending machine, except this one won't steal your quarters
 */
#include <stdio.h>
#include <string.h>

int doYouHaveMoney(float userMoney, float snackMoney){
  if(userMoney < snackMoney){
      return 0; // false, the user does not have the money for the snack
  } else {
      return 1; // true: the user has the money for the snack
  }
}

struct Snack {
  // hardcoding the size of the snack name to be small bc i know the size &
  // "memory conservation" or something.
  char name[8]; 
  float cost;
  int quantity;
};

int main() {
  // var that stores the user's money
  float money;
  
  // var that takes the user's order
  int order; 

  // defining the snacks
  struct Snack Snack1;
  struct Snack Snack2;
  struct Snack Snack3;

  // specify the snacks (structs)
  strcpy(Snack1.name, "appl");
  Snack1.cost = 1.50;
  Snack1.quantity = 21;
  
  strcpy(Snack2.name, "CHEESE!");
  Snack2.cost = 1.50;
  Snack2.quantity = 3;

  strcpy(Snack3.name, "chimken");
  Snack3.cost = 4.00;
  Snack3.quantity = 0;
  
  // snack array
  struct Snack snacks[3];
  snacks[0] = Snack1;
  snacks[1] = Snack2;
  snacks[2] = Snack3;  

  // get money
  printf("Welcome to THE MEAT ZONE!!!! Can I take your order?\n\n");
  printf("How much money do you have? ");
  scanf("%f", &money);
  

  // inventory
  printf("\n");
  // not a for loop bc want specific amt of spaces so formatting looks nice
  printf("0) %s  \t cost: $%.2f\t quantity: %d\n",
          Snack1.name, 
          Snack1.cost, 
          Snack1.quantity);
  printf("1) %s  \t cost: $%.2f\t quantity: %d\n", 
          Snack2.name, 
          Snack2.cost, 
          Snack2.quantity);
  printf("2) %s \t cost: $%.2f\t quantity: %d\n\n", 
         Snack3.name,
         Snack3.cost, 
         Snack3.quantity);
 
  // ordering
  printf("What snack would you like to buy? [0, 1, 2] ");
  scanf("%d", &order);
  printf("\n");
  
  // does the user have the money to afford things    
  // (it runs just once)
  if(order >= 0 && order < 3){
    if(doYouHaveMoney(money, snacks[order].cost) && snacks[order].quantity != 0){
        printf("You bought %s!\n", snacks[order].name);
        money = money - snacks[order].cost;
        printf("You have $%.2f left\n", money);
    } else if(snacks[order].quantity == 0){
        // if there is none of that particular snack
        printf("Sorry! We are out of %s :(\n", snacks[order].name);
    }
     else {
        // if you have no money
        printf("You can't afford that :(\n");  
    }
  } else {
      // case where user enters s/t that's not 0, 1, or 2
      printf("You can't buy that!\n");
  }

  
  return 0;
}
