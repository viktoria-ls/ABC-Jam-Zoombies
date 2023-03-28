/*This file takes care of the view/UI of the game*/
#include <stdlib.h>
#include <math.h>



// Handlers for Main Menu and Ending
void mainMenuHandler() {
	printf("%s", mainMenuScreen);
	char choice = 'x';
	
	while(choice != '1' && choice != '2')
		choice = getch();
		
	system("cls");
	
	if (choice == '2') {
		printf("%s", instructionsScreen);
		getch();
		system("cls");
		mainMenuHandler();	
	}
}

void scoreStatHandler(char *statName, int stat, int multiplier) {
	char statStr[101] = "|               |                    ";
	char statBuffer[4];
	
	strcat(statStr, statName);
	strcat(statStr, " ");
	
	// Adds enough dashes
	int i;
	int statStrLen = strlen(statStr);
	for(i = 0; i < 56 - statStrLen; i++)	// 56 is position before " <stat> <multiplier>"
		strcat(statStr, "-");
	
	// Concatenates stat
	strcat(statStr, " ");
	itoa(stat, statBuffer, 10);
	strcat(statStr, statBuffer);
	
	if(multiplier > 0) {		// no multiplier for speed bonus and total
		// Concatenates multiplier
		strcat(statStr, " X ");
		itoa(multiplier, statBuffer, 10);
		strcat(statStr, statBuffer);
	}
	
	// Adds enough spaces
	statStrLen = strlen(statStr);
	for(i = 0; i < 83 - statStrLen; i++)
		strcat(statStr, " ");
		
	strcat(statStr, "|               |\n");
	
	printf("%s", statStr);
	printf("|               |                                                                  |               |\n");
}

void endHandler(int doorsOpened, int peopleSaved, int zombieHoards, int lives, char* endComment) {
	printf("%s", scoreScreenTop);
	
	// Displays stats
	scoreStatHandler("DOORS OPENED", doorsOpened, 50);
	scoreStatHandler("PEOPLE SAVED", peopleSaved, 100);
	scoreStatHandler("ZOMBIE HOARDS", zombieHoards, 200);
	scoreStatHandler("LIVES", lives, 300);
	
	// Displays total
	printf("|               --------------------------------------------------------------------               |\n");
	printf("|               --------------------------------------------------------------------               |\n");
	printf("|               |                                                                  |               |\n");
	
	int total = doorsOpened*50 + peopleSaved*100 + zombieHoards*200 + lives*300;
	
	scoreStatHandler("TOTAL", total, 0);
	printf("|               --------------------------------------------------------------------               |\n");
	printf("|                                                                                                  |\n");
	printf("|                                                                                                  |\n");
	printf("|                                                                                                  |\n");
	
	// Displays ending comment
	char zoombiesComment[101] = "|               Performance Verdict: ";
	strcat(zoombiesComment, endComment);
	
	int i;
	int zoombiesCommentLen = strlen(zoombiesComment);
	for(i = 0; i < 99 - zoombiesCommentLen; i++)	// 56 is position before " <stat> <multiplier>"
		strcat(zoombiesComment, " ");
	
	strcat(zoombiesComment, "|\n");
	printf("%s", zoombiesComment);
	
	printf("%s", scoreScreenBottom);
	
	getch();
	system("cls");
}

// Handlers for UpperBox
void floorNumHandler(UpperBox *upper, char* event, int speed) {
	char *elevatorOpenings[] = {open0, open1, open2, open3, open4};
	
	strcpy(upper->string, floorHeader0);
	
	char floorNumBuffer[3];
	itoa(upper->floorNum, floorNumBuffer, 10);
	
	if(upper->floorNum < 10)
		strcat(upper->string, "0");
	
	strcat(upper->string, floorNumBuffer);
	
	strcat(upper->string, floorHeader1);

	//strcat(upper->string, elevatorOpenings[upper->doorOpeningLevel]);
	
	if (upper->doorOpeningLevel < 4) // If Door is not fully opened
		strcat(upper->string, elevatorOpenings[upper->doorOpeningLevel]); //Do the open animation
	else {
		if (strcmp(event, "REGAIN_HEARTS") == 0)
			strcat(upper->string, chest);

		else if (strcmp(event, "HUMAN") == 0)
			strcat(upper->string, human);

		else if (strcmp(event, "TIMING_GAME") == 0) {
			if (speed == 3)
				strcat(upper->string, hoard);
			else
				strcat(upper->string, openZ);
		}
		else
			strcat(upper->string, elevatorOpenings[upper->doorOpeningLevel]);
	}
	
	
	upper->floorDown = 0;
}

void doorHandler(UpperBox *upper, char* event, int speed) {
	if (upper->doorAction == 1)		// if door is to be opened
		upper->doorOpeningLevel += 1;
	else							// if door is to be closed
		upper->doorOpeningLevel -= 1;
		
	floorNumHandler(upper, event, speed);
}

// Handlers for LowerBox
void timingGameHandler(LowerBox *lower, int speed) {
	char *box;
	
	if (speed < 3) {
	    char *new = "----------------------------------------------------------------------------------------------------\n"
					"|                                                                                                  |\n"
					"|                                                                                                  |\n"
					"|     Oh no, a zombie appeared! Get ready to attack! Time your attacks well! Press the right       |\n"
					"|     key when the arrow reaches the character!                                                    |\n"
					"|                                                                                                  |\n"
					"|                                                                                                  |\n" 
					"|                                                                                                  |\n"
					"|                                                                                                  |\n"
					"|                                                                Press a button to continue...     |\n"
					"----------------------------------------------------------------------------------------------------\n";
		strcpy(box, new);
	}
	else {
		 char *new = "----------------------------------------------------------------------------------------------------\n"
					"|                                                                                                  |\n"
					"|                                                                                                  |\n"
					"|     A hoard of zombies is approaching!! Prepare yourself! Time your attacks well! Press the      |\n"
					"|     right key when the arrow reaches the character!                                              |\n"
					"|                                                                                                  |\n"
					"|                                                                                                  |\n" 
					"|                                                                                                  |\n"
					"|                                                                                                  |\n"
					"|                                                                Press a button to continue...     |\n"
					"----------------------------------------------------------------------------------------------------\n";
		strcpy(box, new);
	}
	strcpy(lower->string, box);
}

void doorChoiceHandler(LowerBox *lower) {
	// Temporary default text for door choice; would need to randomly assign a floor number as "with person" or not
	char *box = 	"----------------------------------------------------------------------------------------------------\n"
					"|                                                                                                  |\n"
					"|                                                                                                  |\n"
					"|     To face the dangers outside the door or bring yourself to safety... what do you do?          |\n"
					"|     (Press 1 or 2 to choose)                                                                     |\n"
					"|                                                                                                  |\n"
					"|     [1] OPEN DOOR                                                                                |\n" 
					"|     [2] SKIP FLOOR                                                                               |\n"
					"|                                                                                                  |\n"
					"|                                                                                                  |\n"
					"----------------------------------------------------------------------------------------------------\n";
	
	strcpy(lower->string, box);
}

void noDoorChoiceHandler(LowerBox *lower) {
	// Temporary default text for door choice; would need to randomly assign a floor number as "with person" or not
	char *box = 	"----------------------------------------------------------------------------------------------------\n"
					"|                                                                                                  |\n"
					"|                                                                                                  |\n"
					"|     Oh no, the elevator broke down! Prepare yourself before the door opens!                      |\n"
					"|                                                                                                  |\n"
					"|                                                                                                  |\n"
					"|                                                                                                  |\n" 
					"|                                                                                                  |\n"
					"|                                                                                                  |\n"
					"|                                                                Press a button to continue...     |\n"
					"----------------------------------------------------------------------------------------------------\n";
	
	strcpy(lower->string, box);
}

void regainHeartHandler(LowerBox *lower) {
	// Temporary default text for door choice; would need to randomly assign a floor number as "with person" or not
	char *box = 	"----------------------------------------------------------------------------------------------------\n"
					"|                                                                                                  |\n"
					"|                                                                                                  |\n"
					"|     You found food and supplies! You gained 1 extra heart.                                       |\n"
					"|                                                                                                  |\n"
					"|                                                                                                  |\n"
					"|                                                                                                  |\n" 
					"|                                                                                                  |\n"
					"|                                                                                                  |\n"
					"|                                                                Press a button to continue...     |\n"
					"----------------------------------------------------------------------------------------------------\n";
	
	strcpy(lower->string, box);
}

void humanHandler(LowerBox *lower) {
	// Temporary default text for door choice; would need to randomly assign a floor number as "with person" or not
	char *box = 	"----------------------------------------------------------------------------------------------------\n"
					"|                                                                                                  |\n"
					"|                                                                                                  |\n"
					"|     You found a survivor!                                                                        |\n"
					"|                                                                                                  |\n"
					"|                                                                                                  |\n"
					"|                                                                                                  |\n" 
					"|                                                                                                  |\n"
					"|                                                                                                  |\n"
					"|                                                                Press a button to continue...     |\n"
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
		for (j = 0; j < 88 - strlen(narrationLine); j++)		// 88 is (total width - 2 borders - 10 padding)
			strcat(lower->string, " ");
		
		strcat(lower->string, "     |\n");
	}

	char *bottom =  "|                                                                                                  |\n"
					"|                                                                Press a button to continue...     |\n"
					"----------------------------------------------------------------------------------------------------\n";
					
	strcat(lower->string, bottom);
}

// Calls handler, clears screen, prints updated UpperBox and LowerBox
void updateGameUI(UpperBox *upper, LowerBox *lower, int speed) {	

	if (lower->isWaitingForDoor == 1) {		// if door needs to be opened or closed
		doorHandler(upper, lower->type, speed);
		Sleep(300);
		
		if((upper->doorAction == 0 && upper->doorOpeningLevel == 0) || (upper->doorAction == 1 && upper->doorOpeningLevel == 4)) // if door finally fully opens/closes
			lower->isWaitingForDoor = 0;
	}
	
	if(upper->floorDown == 1)
		floorNumHandler(upper, lower->type, speed);
	
	// Calls appropriate handler
	if (strcmp(lower->type, "TIMING_GAME") == 0)
		timingGameHandler(lower, speed);
	else if (strcmp(lower->type, "DOOR_CHOICE") == 0)
		doorChoiceHandler(lower);
	else if (strcmp(lower->type, "NARRATION") == 0)
		narrationHandler(lower);
	else if (strcmp(lower->type, "NO_CHOICE") == 0)
		noDoorChoiceHandler(lower);
	else if (strcmp(lower->type, "REGAIN_HEARTS") == 0)
		regainHeartHandler(lower);
	else if (strcmp(lower->type, "HUMAN") == 0)
		humanHandler(lower);
		
	system("cls");
	printf("%s", upper->string);
	printf("%s", lower->string);
}
