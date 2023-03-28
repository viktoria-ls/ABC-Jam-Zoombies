/* This file contains aliases and structs (and some functions for now) */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <string.h>
#include <conio.h>





/* Chance Generators */
int brokenElevChance() {
    int chance = rand () % 100;

    //40 percent chance of borken elev
    if (chance < 40)
        return 1;
    else
        return 0;
}

int openElevChance() {
    int chance = rand () % 100;

    //Regain Hearts
    if (chance < 20)
        return 1;
    //Zombie
    else if (chance >= 20 && chance < 70)
        return 2;
    //People
    else if (chance >= 70 && chance < 100)
        return 3;
}


int speedGen(int floor) {
    int intervals = MAX_FLOOR / 3;

    if (floor <= MAX_FLOOR && floor >= MAX_FLOOR-intervals)
        return 1;
    if (floor <= MAX_FLOOR - intervals - 1 && floor >= MAX_FLOOR - intervals * 2)
        return 2;
    if (floor <= MAX_FLOOR - intervals * 2 - 1 && floor >= MAX_FLOOR - intervals * 3)
        return 3;
}


void randomLetters(int numLetters, String *letters) {
	int i;

	for (i = 0; i < numLetters; i++) {
		char randomletter = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"[rand () % 26];
		strncat(*letters, &randomletter, 1);
	}
}

/* Add spaces for the arrow to move*/
void addSpace(Line *space) {
    char c = ' ';
	strncat(*space, &c, 1);
}

/*Reset all the spaces */
void resetSpace(Line *space) {
	Line newLine = "";
	memmove(*space, newLine, sizeof(newLine));
}

/*Remove spaces for the arrow to move back */
void removeSpace(Line space) {
	space[strlen(space)-1] = '\0';
}


void generateBar(String *bar, int lengthBar) {
    int i;

    for (i = 0; i < lengthBar; i++) {
        char c = '-';
	    strncat(*bar, &c, 1);
    }
}


void generateStringSpace(Line *stringSpace, int limit) {
    int i;

    for (i = 0; i < limit; i++) {
        addSpace(&*stringSpace);
    }
}

int checkIfWithinBarArea(int lengthBar, int cursor, int numSpaces) {
    int start = numSpaces;
    int end = numSpaces + lengthBar;

    if (cursor >= start && cursor <= end)
        return 1;
    else
        return 0;
}

/*The "Timing Game" l oop that iterates through the characters of param toPrint*/
void timingGameRun(int speed, int *life, UpperBox *upper) {
	String toPrint = "";
	int numLetters;
	int startSpace;
	int endSpace;
	int x; // Counter for the space
    int index = 0; // Index of the character of the string to be printed
    int randPos; // Location of the character
    int bFlag = 0; // flag if the letter is completed
    char c; // Character to print on screen
    char input; // Input of keyboard
	Line space = ""; // The spaces for the arrows
    Line stringSpace = ""; // The spaces for the character and bar
    String bar = ""; // Bar for the level
	int lengthBar;
	int arrowDirection = 0; //0 going right, 1 going left

	//FOR TESTING
	int touched = 0;

	/*CountDown Timer Variables*/
	unsigned int x_minutes=0;
	unsigned int x_seconds=0;
	unsigned int x_milliseconds=0;
	unsigned int count_down_time_in_secs=0;
	unsigned int time_left=0;
	clock_t x_startTime,x_countTime;

	if (speed==1) {
		startSpace = 30;
		endSpace = 70;
		numLetters = 3;
		lengthBar = 12;
		count_down_time_in_secs=30;
	}
	if (speed==2) {
		startSpace = 35;
		endSpace = 65;
		numLetters = 4;
		lengthBar = 8;
		count_down_time_in_secs=20;
	}
		
	if (speed==3) {
		startSpace = 40;
		endSpace = 60;
		numLetters = 5;
		lengthBar = 4;
		count_down_time_in_secs=15;
	}
	x = startSpace;
	randomLetters(numLetters, &toPrint);
	generateBar(&bar, lengthBar); // Generates the bar
	generateStringSpace(&space, startSpace);
		


	x_startTime = clock();  // start clock
    time_left = count_down_time_in_secs - x_seconds;   // update timer
	while (index < strlen(toPrint) && *(life) > 0) {
        c = toPrint[index];
        randPos = rand() % (endSpace - startSpace - numLetters) + startSpace;
        generateStringSpace(&stringSpace, randPos-1);

        while(*life > 0 && bFlag == 0 && time_left > 0) {
		/* Timer */
		x_countTime=clock(); // update timer difference
		x_milliseconds=x_countTime-x_startTime;
		x_seconds=(x_milliseconds/(CLOCKS_PER_SEC))-(x_minutes*60);
		x_minutes=(x_milliseconds/(CLOCKS_PER_SEC))/60;
		time_left=count_down_time_in_secs-x_seconds; // subtract to get difference 

            if (_kbhit()) {
                input = _getch();
                input = toupper(input);
				touched = x;
                
                // If it hits the correct area and is the right letter
                if (input == c && checkIfWithinBarArea(lengthBar, touched, randPos) == 1) {
                    PlaySound("Audio/gunshot.wav", NULL, SND_FILENAME | SND_ASYNC);
                    bFlag = 1;
                }
                else {
                    PlaySound("Audio/hurt.wav", NULL, SND_FILENAME | SND_ASYNC);
                    *life -= 1;
                }
            }
            else {
				/*Test Life*/
				printf("Life: %d\n\n", *life);
				printf("Timer: %d\n", time_left);

	            printf("%s", upper->string);

				printf(space);
				printf("|\n");
				printf(space);
				printf("|\n");
				printf(space);
				printf("|\n");
				printf(space);
				printf("v\n\n");

				printf(stringSpace);
				printf(bar);
				printf("\n");
				printf(stringSpace);
				printf("%c\n", c);


				printf(space);
				printf("^\n");
				printf(space);
				printf("|\n");
				printf(space);
				printf("|\n");
				printf(space);
				printf("|\n");

				Sleep(15);
				system("cls");

				/* Determine Arrow Direction */
				if (x==endSpace)
					arrowDirection = 1;
				else if (x==startSpace)
					arrowDirection = 0;

				/* Add Space or Remove Space*/
				if (arrowDirection == 1) {
					x -= 1;
					removeSpace(space);
				}
				else {
					x += 1;
					addSpace(&space);
				}
            }
        }
        bFlag = 0; // Reset letter flag
        index += 1;

		if (time_left == 0)
			*life = 0;

        // reset the space for the strings
        resetSpace(&stringSpace);
    }
    // A lil brute forcish

    if (*life > 0) {
        strcpy(upper->string, floorHeader0);
	    char floorNumBuffer[3];
	    itoa(upper->floorNum, floorNumBuffer, 10);
	    if(upper->floorNum < 10)
		    strcat(upper->string, "0");
        strcat(upper->string, floorNumBuffer);
	    strcat(upper->string, floorHeader1);

        strcat(upper->string, open4);

        printf("%s", upper->string);
        if (speed < 3)
            printf("%s", victory);
        else
            printf("%s", victory2);
        PlaySound("Audio/Zombie2.wav", NULL, SND_FILENAME | SND_ASYNC);
    }
    else {
        printf("%s", die);
        PlaySound("Audio/dead.wav", NULL, SND_FILENAME | SND_ASYNC);
    }
    getch();
}

