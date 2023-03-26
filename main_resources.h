/* This file contains aliases and structs (and some functions for now) */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <string.h>
#include <conio.h>

#define WIDTH (100)
#define HEIGHT (41)
typedef char Line[WIDTH + 1];
typedef char String[31];





typedef char UpperBoxString[WIDTH * 30 + 30 + 1];		// (100 width * 30 lines) + 30 newlines + 1 null terminator
typedef char LowerBoxString[WIDTH * 11 + 11 + 1];		// (100 width * 11 lines) + 11 newlines + 1 null terminator

// Struct/s for UpperBox
typedef struct {
	int floorNum;
	int doorOpeningLevel;	// 0 is fully closed, 4 is fully opened
	int doorAction;		// 1 if needs to be open, 0 if needs to be closed
	int floorDown;		// 1 if floorNum display needs to change (down)
	char *image;
	UpperBoxString string;
} UpperBox;

// Struct/s for LowerBox
typedef struct {
	char *content;
	int hasNextPage; 	// 1 if content overflows to next narration page
} NarrationLowerBox;

typedef struct {
	int speed;			// 1 to 3 for different speeds
} TimingGameLowerBox;

typedef struct {
	char *type;
	int isWaitingForDoor;
	LowerBoxString string;						// String representation of LowerBox
	NarrationLowerBox narrationLowerBox;		// NarrationLowerBox data if type == NARRATION
	TimingGameLowerBox timingGameLowerBox;		// TimingGameLowerBox data if type == TIMING_GAME
} LowerBox;






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
void timingGame(String toPrint, int speed, int *life, int lengthBar) {
    srand(time(NULL));
	int x = 0; // Counter for the space
    int index = 0; // Index of the character of the string to be printed
    int randPos; // Location of the character
    int bFlag = 0; // flag if the letter is completed
    char c; // Character to print on screen
    char input; // Input of keyboard
	Line space = ""; // The spaces for the arrows
    Line stringSpace = ""; // The spaces for the character and bar
    String bar = ""; // Bar for the level
    generateBar(&bar, lengthBar); // Generates the bar

	while (index < strlen(toPrint) && *(life) > 0) {
        c = toPrint[index];
        randPos = rand() % 60;
        generateStringSpace(&stringSpace, randPos-1);

        while(*life > 0 && bFlag == 0) {
            if (_kbhit()) {
                input = _getch();
                input = toupper(input);
                
                // If it hits the correct area and is the right letter
                if (input == c && checkIfWithinBarArea(lengthBar, x, randPos) == 1)
                    bFlag = 1;
                else
                    *life -= 1;
            }
            else {
            printf("Life: %d", *life);
            printf("\n\n\n");

            addSpace(&space);
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

            Sleep(speed);
            system("cls");

            if (x == WIDTH-1) {
                x = 0;
                resetSpace(&space);
            }
            else
                x += 1;
            }
        }
        bFlag = 0; // Reset letter flag
        index += 1;

        // reset the space for the strings
        resetSpace(&stringSpace);
    }

    // Win Or Lose
    if (*life == 0)
        printf("\nYOU SUCK!\n");
    else
        printf("\nNOICE\n");

}

