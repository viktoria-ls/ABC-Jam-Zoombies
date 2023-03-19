/*
This file contains all constants, aliases, structs and function
*/

#include <stdio.h>
#include <windows.h>
#include <conio.h>

#define SCREEN_SIZE (120)
typedef char Line[SCREEN_SIZE + 1];
typedef char String[31];

/*Clears the current screen and replaces it with*/
void updateScreen() {
	printf("ZOOMBIES\n");
	char testChar = getch();
	
	//Sleep(3000);
	system("cls");
	printf("You entered: %c\n", testChar);
}

/*The "Timing Game" loop that iterates through the characters of param toPrint*/
void timingGame(String toPrint, float speed) {
	
}
