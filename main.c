/* This file has the defines the initial parameters of the game and contains the game loop */

#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <windows.h>
#include <time.h>

#include "assets.h"
#include "view.h"
#include "main_resources.h"


void floorDown(UpperBox *upper, int *currentFloor) {
	upper->floorDown = 1;
	upper->floorNum -= 1;
	(*currentFloor) -= 1;
}

int main() {
	srand(time(NULL));
	while(1) {
		PlaySound("Audio/opening.wav", NULL, SND_FILENAME | SND_ASYNC);
		mainMenuHandler();
		PlaySound("Audio/game.wav", NULL, SND_FILENAME | SND_ASYNC);
	
		int life = 3;
		int currentFloor = MAX_FLOOR;

		// Chance Variables //
		int elevIsBroken = 0;
		int isEvent = 0;
		
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
		timingGame.speed = 1;
		
		lower.narrationLowerBox = narration;
		lower.timingGameLowerBox = timingGame;
		
		// Game Loop
		while(currentFloor >= 1 && life > 0) {
			updateGameUI(&upper, &lower, timingGame.speed);
			
			if(lower.isWaitingForDoor == 0) {		// If door is not currently opening/closing
				if (strcmp(lower.type, "TIMING_GAME") == 0) {
					PlaySound("Audio/Zombie1.wav", NULL, SND_FILENAME | SND_ASYNC);
					getch();
					system("cls");
					
					timingGame.speed = speedGen(upper.floorNum);
					timingGameRun(timingGame.speed, &life, &upper);	// Used to simulate finishing the timing game for now
					PlaySound("Audio/game.wav", NULL, SND_FILENAME | SND_ASYNC);

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
						isEvent = openElevChance();

						if (isEvent == 2)
							lower.type = "TIMING_GAME";
						else if (isEvent == 1)
							lower.type = "REGAIN_HEARTS";
						else
							lower.type = "HUMAN";

						lower.isWaitingForDoor = 1;
						upper.doorAction = 1;
					}
					else if (choice == '2') {
						lower.type = "NARRATION";
						floorDown(&upper, &currentFloor);
					}
				}
				else if (strcmp(lower.type, "NO_CHOICE") == 0) {
					getch();
					isEvent = openElevChance();

					if (isEvent == 2)
						lower.type = "TIMING_GAME";
					else if (isEvent == 1)
						lower.type = "REGAIN_HEARTS";
					else
						lower.type = "HUMAN";

					lower.isWaitingForDoor = 1;
					upper.doorAction = 1;
				}
				else if (strcmp(lower.type, "NARRATION") == 0) {// Assumes type == NARRATION always means transition between timing game and door choice
					getch();
					lower.type = "DOOR_CHOICE";
					elevIsBroken = openElevChance();

					if (elevIsBroken == 1)
						lower.type = "NO_CHOICE";
				}
				else { // For other events
					if (strcmp(lower.type, "HUMAN") == 0) {
						//ADd score
						
						PlaySound("Audio/human.wav", NULL, SND_FILENAME | SND_ASYNC);
					}

					if (strcmp(lower.type, "REGAIN_HEARTS") == 0) {
						life += 1;
						PlaySound("Audio/loot.wav", NULL, SND_FILENAME | SND_ASYNC);
					}

					getch();
					upper.doorAction = 0;			// Door is to be closed because timing game is over
					lower.isWaitingForDoor = 1;		// Has to wait for door to close again
					lower.type = "NARRATION";
					floorDown(&upper, &currentFloor);
					PlaySound("Audio/game.wav", NULL, SND_FILENAME | SND_ASYNC);
				}

			}
		}
		
		system("cls");
		PlaySound("Audio/ending.wav", NULL, SND_FILENAME | SND_ASYNC);
		endHandler(doorsOpened, peopleSaved, zombieHoards, life, speedBonus, endComment);	
	}
	return 0;
}
