/*
 * password.c
 * Author: Audrey Yang
 * Date: 1/27/2022
 * Runs a game of Rock, Paper, Scissors, you vs. the computer. 
 */

#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <ctype.h>

// function that will generate the random moves
const char* computerMove(){
  int ref;

  ref = rand() % 3;
  // printf("%d\n", ref); // todo: remove/comment out
  
  if(ref == 0){
    return "rock";
  } else if (ref == 1) {
    return "paper";
  } else {
    return "scissors";
  }
  
}

// bluck of the calculations here which i decided was a good idea 
// for some reason.
int rpcRound(const char* playerMove, const char* aiMove){
 // output message of "move advantages"
 // originally tried to be clever by grouping same out come (e.g., rock beat
 // paper no matter if player vs ai won) and then compare using strcmp
 // TIL/remembered that C handles logic operators Differently than java. wahoo.
  if(strcmp(playerMove, "rock") == 0 && strcmp(aiMove, "paper") == 0){
    // player beats ai with paper over rock
    printf("Paper covers Rock\n"); 
    return 2;
  } else if(strcmp(playerMove, "paper") == 0 && strcmp(aiMove, "rock") == 0){
    // ai beats player with paper over rock
    printf("Paper covers Rock\n");
    return 1;
  } else if(strcmp(playerMove, "paper") == 0 && strcmp(aiMove, "scissors") == 0){
    // ai beats player with scissors over paper
    printf("Scissors cut Paper\n");
    return 2;
  } else if(strcmp(playerMove, "scissors") == 0 && strcmp(aiMove, "paper") == 0){
    // player beats ai with scissors over paper
    printf("Scissors cuts Paper\n");
    return 1;
  } else if(strcmp(playerMove, "scissors") == 0 && strcmp(aiMove, "rock") == 0){
    // ai beats player with rock over scissors
    printf("Rock crusches Scissors\n");
    return 2;
  } else if(strcmp(playerMove, "rock") == 0 && strcmp(aiMove, "scissors") == 0){
    // player beats ai with rock over scissors
    printf("Rock crushes Scissors\n");
    return 1;
  } else {
    // ai and player choose the same thing
    printf("It's a Tie!\n");
    return 0;
  }
}

int main() {
  srand(time(0));
  
  int rounds;
  int playerWins, aiWins;
  char playerMove[10]; // it could be size of 9 but like. safety :(
  const char* aiMove;
  int results;

  // known bug: typing in a string instead of int for # of rounds
  // leads to weird things happening
  printf("Welcome to Rock, Paper, Scissors!\n");
  printf("How many rounds do you want to play? ");
  scanf("%d", &rounds);
  printf("\n");

  playerWins = 0;
  aiWins = 0;

  // running the game
  for(int i = 0; i < rounds; i++){
    printf("Which do you choose? Rock, Paper, or Scissors? ");
    scanf("%8s", playerMove); // get player choice
  
    // make the player's move lowercase so i can type less
    for(int j = 0; j < strlen(playerMove); j++){
      if(playerMove[j] >= 'A' && playerMove[j] <= 'Z'){
        // printf("converting!\n");
        playerMove[j] = playerMove[j] + 32;
      } 
    }
 
    aiMove = computerMove(); // computer makes move
  
    if(strcmp(playerMove, "rock") != 0 && strcmp(playerMove, "paper") != 0 
       && strcmp(playerMove, "scissors") != 0){
      // if player says some wiggedy-wack, tell them they said wiggedy-wack
      printf("You entered an invalid choice: %s\n", playerMove); 
    } else{
      printf("AI chose %s\n", aiMove);
      
      // compare the two 
      results = rpcRound(playerMove, aiMove);
      if(results == 1){
        playerWins = playerWins + 1;
        printf("Player Won\n");
      }
      if(results == 2){
        aiWins = aiWins + 1;
        printf("AI Won\n");
      }
    }
    printf("AI Score: %d\t Player Score: %d\n\n", aiWins, playerWins);
  }

  // end results
   if(playerWins > aiWins) {
      printf("Player wins!\n");
   } else if (aiWins > playerWins) {
      printf("AI wins!\n");   
   } else {
      printf("It's a Tie!\n");
   }

  return 0;
}

