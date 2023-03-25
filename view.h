/*This file takes care of the view/UI of the game*/
#include <stdlib.h>
#include "assets.h"

// Handlers for UpperBox
void floorNumHandler(UpperBox *upper) {
	char *elevatorOpenings[] = {open0, open1, open2, open3, open4};
	
	strcpy(upper->string, floorHeader0);
	
	char floorNumBuffer[3];
	itoa(upper->floorNum, floorNumBuffer, 10);
	
	if(upper->floorNum < 10)
		strcat(upper->string, "0");
	
	strcat(upper->string, floorNumBuffer);
	
	strcat(upper->string, floorHeader1);
	strcat(upper->string, elevatorOpenings[upper->doorOpeningLevel]);
	
	upper->floorDown = 0;
}

void doorHandler(UpperBox *upper) {
	if (upper->doorAction == 1)		// if door is to be opened
		upper->doorOpeningLevel += 1;
	else							// if door is to be closed
		upper->doorOpeningLevel -= 1;
		
	floorNumHandler(upper);
}

// Handlers for LowerBox
void timingGameHandler(LowerBox *lower) {
	char *box = 	"----------------------------------------------------------------------------------------------------\n"
					"|                                                                                                  |\n"
					"|                                                                                                  |\n"
					"|     Placeholder for the actual timing game. Don't mind this heheh                                |\n"
					"|     Pressing a button here will simulate ending the timing game (won't decrease lives)           |\n"
					"|                                                                                                  |\n"
					"|     The next thing to change on the screen is the elevator door closing and a transition         |\n" 
					"|     narration LowerBox to go to next door choice                                                 |\n"
					"|                                                                                                  |\n"
					"|                                                                Press a button to continue...     |\n"
					"----------------------------------------------------------------------------------------------------\n";
	
	strcpy(lower->string, box);
}

void doorChoiceHandler(LowerBox *lower) {
	// Temporary default text for door choice; would need to randomly assign a floor number as "with person" or not
	char *box = 	"----------------------------------------------------------------------------------------------------\n"
					"|                                                                                                  |\n"
					"|                                                                                                  |\n"
					"|     You hear screams outside the elevator door! What do you do?                                  |\n"
					"|     (Press 1 or 2 to choose)                                                                     |\n"
					"|                                                                                                  |\n"
					"|     [1] OPEN DOOR                                                                                |\n" 
					"|     [2] SKIP FLOOR                                                                               |\n"
					"|                                                                                                  |\n"
					"|                                                                                                  |\n"
					"----------------------------------------------------------------------------------------------------\n";
	
	strcpy(lower->string, box);
}

void narrationHandler(LowerBox *lower) {
	char *top = 	"----------------------------------------------------------------------------------------------------\n"
					"|                                                                                                  |\n"
					"|                                                                                                  |\n";
					
	strcpy(lower->string, top);

	int i;
	for (i = 0; i < 5; i++) {
		char *narrationLine = lower->narrationLowerBox.lines[i];		// Gets each line in narration
		
		strcat(lower->string, "|     ");
		strcat(lower->string, narrationLine);
		
		int j;
		for (j = 0; j < 88 - strlen(narrationLine); j++)
			strcat(lower->string, " ");
		
		strcat(lower->string, "     |\n");
	}

	char *bottom =  "|                                                                                                  |\n"
					"|                                                                Press a button to continue...     |\n"
					"----------------------------------------------------------------------------------------------------\n";
					
	strcat(lower->string, bottom);
}

// Calls handler, clears screen, prints updated UpperBox and LowerBox
void updateGameUI(UpperBox *upper, LowerBox *lower) {
	if (lower->isWaitingForDoor == 1) {		// if door needs to be opened or closed
		doorHandler(upper);
		Sleep(300);
		
		if((upper->doorAction == 0 && upper->doorOpeningLevel == 0) || (upper->doorAction == 1 && upper->doorOpeningLevel == 4)) // if door finally fully opens/closes
			lower->isWaitingForDoor = 0;
	}
	
	if(upper->floorDown == 1)
		floorNumHandler(upper);
	
	// Calls appropriate handler
	if (strcmp(lower->type, "TIMING_GAME") == 0)
		timingGameHandler(lower);
	else if (strcmp(lower->type, "DOOR_CHOICE") == 0)
		doorChoiceHandler(lower);
	else if (strcmp(lower->type, "NARRATION") == 0)
		narrationHandler(lower);
	
	system("cls");
	
	printf("%s", upper->string);
	printf("%s", lower->string);
}
