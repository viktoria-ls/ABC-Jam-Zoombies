/* This file has the defines the initial parameters of the game and contains the game loop */

#include <stdio.h>
#include <conio.h>
#include <string.h>

#include "main_resources.h"
#include "view.h"

void floorDown(UpperBox *upper, int *currentFloor) {
	upper->floorDown = 1;
	upper->floorNum -= 1;
	(*currentFloor) -= 1;
}

int main() {
	while(1) {
		mainMenuHandler();
	
		int life = 3;
		int currentFloor = 15;
		
		// Variables to store score values
		int doorsOpened = 0;
		int peopleSaved = 0;
		int zombieHoards = 0;
		int speedBonus = 20;
		char *endComment = "You're our hero! Play again?";
	
		// UpperBox(floorNum, doorOpeningLevel, doorAction, floorDown, image, string)
		UpperBox upper = {currentFloor, 4, 0, 0, "", ""};
		
		// LowerBox(type, isWaitingForDoor, string, narrationLowerBox, timingGameLowerBox);
		LowerBox lower;
		lower.type = "NARRATION";
		lower.isWaitingForDoor = 1;
		strcpy(lower.string, "");
		
		NarrationLowerBox narration;
		// each line should have max 88 characters; below is sample narration text
		narration.lines[0] = "This is just some default narration text. This is supposed to say something like,";
		narration.lines[1] = "\"The zombie falls as you put a bullet in its head (dam thats gory)...\" like that.";
		narration.lines[2] = "This is still pretty manual, maybe we can make a better system for this so that the";
		narration.lines[3] = "overflow from a text would go to the next page and you would get to go to the next page";
		narration.lines[4] = "by pressing a button. Stuff like that. Might be optional if we're pressed for time.";
		
		TimingGameLowerBox timingGame;
		timingGame.speed = 0;
		
		lower.narrationLowerBox = narration;
		lower.timingGameLowerBox = timingGame;
		
		// Game Loop
		while(currentFloor >= 1 && life > 0) {
			updateGameUI(&upper, &lower);
			
			if(lower.isWaitingForDoor == 0) {		// If door is not currently opening/closing
				if (strcmp(lower.type, "TIMING_GAME") == 0) {
					getch();	// Used to simulate finishing the timing game for now
					
					upper.doorAction = 0;			// Door is to be closed because timing game is over
					lower.isWaitingForDoor = 1;		// Has to wait for door to close again
					lower.type = "NARRATION";
					floorDown(&upper, &currentFloor);
					
				}
				else if (strcmp(lower.type, "DOOR_CHOICE") == 0) {
					char choice = 'x';
					while (choice != '1' && choice != '2')
						choice = getch();
					if (choice == '1') {
						lower.type = "TIMING_GAME";
						lower.isWaitingForDoor = 1;
						upper.doorAction = 1;
					}
					else if (choice == '2') {
						lower.type = "NARRATION";
						floorDown(&upper, &currentFloor);
					}
				}
				else if (strcmp(lower.type, "NARRATION") == 0) {		// Assumes type == NARRATION always means transition between timing game and door choice
					getch();
					lower.type = "DOOR_CHOICE";
				}
			}
		}
		
		system("cls");
		endHandler(doorsOpened, peopleSaved, zombieHoards, life, speedBonus, endComment);	
	}
	return 0;
}
