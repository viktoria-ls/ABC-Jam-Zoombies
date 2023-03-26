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
		
		NarrationLowerBox narration;
		narration.content = "As the elevator doors slid open, James hesitated for a moment. The lobby was eerily quiet, and something about the atmosphere gave him chills. He shook it off and stepped inside, pressing the button for the fifteenth floor. But as the elevator started to ascend, he heard a low growling noise. His heart racing, he looked up to see a group of zombies staggering towards him. Panic set in as James realized he was trapped in the elevator with no way out. He quickly assessed his options, searching for a weapon to defend himself. But all he had was his briefcase.";
		narration.hasNextPage = 0;

		TimingGameLowerBox timingGame;
		timingGame.speed = 0;
		
		// LowerBox(type, isWaitingForDoor, string, narrationLowerBox, timingGameLowerBox);
		LowerBox lower = {"NARRATION", 1, "", narration, timingGame};
		
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
					if(lower.narrationLowerBox.hasNextPage == 0)
						lower.type = "DOOR_CHOICE";
				}
			}
		}
		
		system("cls");
		endHandler(doorsOpened, peopleSaved, zombieHoards, life, speedBonus, endComment);	
	}
	return 0;
}
