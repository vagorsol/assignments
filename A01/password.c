/*
 * password.c
 * Author: Audrey Yang
 * Date: 1/22/2022
 * Generates the worst possible passwords when given a word by the user
 */

#include <stdio.h>

int main(){
  char passwrd[128]; 
  printf("Enter a word: ");

  // get the password (should just only take the first 128 characters
  scanf("%128s", passwrd); 
  
  // change the password
  for(int i = 0; i < 128; i++) {
    // if the character is an 'e', make it a '3'
    if(passwrd[i] == 'e' || passwrd[i] == 'E'){
        passwrd[i] = '3';
    } 
    // if the character is an 'l', make it a '1'
    if(passwrd[i] == 'l' || passwrd[i] == 'L'){
        passwrd[i] = '1';
    } 
    // if the character is an 'a', make it a '@'
    if(passwrd[i] == 'a' || passwrd[i] == 'A'){
        passwrd[i] = '@';
    } 
  }
  
  printf("Your bad password is %s\n", passwrd);
	return 0;
}
