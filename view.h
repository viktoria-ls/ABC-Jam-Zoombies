/*This file takes care of the view/UI of the game*/
#include <stdlib.h>
#include <math.h>
#include "assets.h"

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

void endHandler(int doorsOpened, int peopleSaved, int zombieHoards, int lives, int speedBonus, char* endComment) {
	printf("%s", scoreScreenTop);
	
	// Displays stats
	scoreStatHandler("DOORS OPENED", doorsOpened, 50);
	scoreStatHandler("PEOPLE SAVED", peopleSaved, 100);
	scoreStatHandler("ZOMBIE HOARDS", zombieHoards, 200);
	scoreStatHandler("LIVES", lives, 300);
	scoreStatHandler("SPEED BONUS", speedBonus, 0);
	
	// Displays total
	printf("|               --------------------------------------------------------------------               |\n");
	printf("|               --------------------------------------------------------------------               |\n");
	printf("|               |                                                                  |               |\n");
	
	int total = doorsOpened*50 + peopleSaved*100 + zombieHoards*200 + lives*300 + speedBonus;
	
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
					"|                                                                 Press any key to continue...     |\n"
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
	char *content = lower->narrationLowerBox.content;
	int maxLineLen = WIDTH - 2 - 10;
	char line[maxLineLen + 1];
	
	String word;
	int linesMade = 0;
	int charsRead = 0;
	for (i = 0; i < strlen(content); i++) {
		if(linesMade == 5) {
			lower->narrationLowerBox.hasNextPage = 1;
			break;
		}
		if(content[i] != ' ')
			strcat(word, (char[2]){(char)content[i]});
			
		else {
			if(strlen(word) + strlen(line) <= maxLineLen) {			// if adding word fits in line
				strcat(line, word);
				if(strlen(line) + 1 <= maxLineLen)
					strcat(line, " ");
				charsRead += strlen(word) + 1;
				strcpy(word, "");
			}
			else {		// if adding word doesnt fit in line
				int j;
				int spacesToFill = maxLineLen - strlen(line);
				for (j = 0; j < spacesToFill; j++)		// fill rest of needed space
					strcat(line, " ");
				
				strcat(lower->string, "|     ");
				strcat(lower->string, line);
				strcat(lower->string, "     |\n");
				
				strcpy(line, "");
				
				linesMade++;
				i--;
			}
		}
	}
	
	int j;
	for (j = linesMade; j < 5; j++) {
		strcat(lower->string, "|                                                                                                  |\n");
		lower->narrationLowerBox.hasNextPage = 0;	
	}

	char *bottom =  "|                                                                                                  |\n"
					"|                                                                 Press any key to continue...     |\n"
					"----------------------------------------------------------------------------------------------------\n";
	strcat(lower->string, bottom);
	
	
					
	if(lower->narrationLowerBox.hasNextPage == 1) {
		int remainingContentLen = strlen(content) - charsRead;
		char remainingContent[remainingContentLen + 1];
		strncpy(remainingContent, content + charsRead, remainingContentLen);
		lower->narrationLowerBox.content = remainingContent;
	}
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
