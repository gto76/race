#include <stdio.h>
#include <string.h>

#include "util.h"
#include "output.h"

#define SB_LOCATION_X 27
#define SB_LOCATION_Y 9
#define SB_WIDTH 26
#define SB_HEIGHT 6

void setScoreboard(void);
void printMatrixOnBoardXY(char const **matrix, int x, int y);
void printStringOnBoardXY(char const s[], int x, int y);
int coordinatesAreInsideBoard(int x, int y);
int stringOverRightEdge(char const s[], int x);
void printCharOnBoardXY(char const c, int x, int y);
void printScoreboardBuffer(void);
void eraseScoreboard(void);
void sb_printChar(char const c, int x, int y);
void sb_printStringAndWriteToBuffer(char const s[], int x, int y);
void sb_printString(char const s[], int x, int y);
void sb_writeToBuffer(char const s[], int x, int y);

/////////////////////

static char buffer[SB_HEIGHT][SB_WIDTH];

////// PUBLIC ///////

void setScoreboard() {
	int i, j;
	for (i = 0; i < SB_HEIGHT; i++) {
		for (j = 0; j < SB_WIDTH-1; j++) {
			buffer[i][j] = ' ';
		}
		buffer[i][SB_WIDTH-1] = '\0';
	}
}

//////////////////

void printMatrixOnBoardXY(char const **matrix, int x, int y) {
	char const **mp;
	for (mp = matrix; *mp != NULL; mp++) {
		printStringOnBoardXY(*mp, x, y++);
	}
	fflush(stdout);
}

void printStringOnBoardXY(char const s[], int x, int y) {
	if (!coordinatesAreInsideBoard(x,y)) {
		return;
	}
	if (stringOverRightEdge(s, x)) {
		return;
		//int newWidth = SB_LOCATION_X + SB_WIDTH - x;
		//char subArray[newWidth+1];
		//copyArray(&s, &subArray, newWidth);
	} else {
		sb_printStringAndWriteToBuffer(s, x, y);
	}
}

int coordinatesAreInsideBoard(int x, int y) {
	return x >= 0 && x < SB_WIDTH && y >= 0 && y < SB_HEIGHT;
}

int stringOverRightEdge(char const s[], int x) {
	return SB_WIDTH < strlen(s) + x;
}

void printCharOnBoardXY(char c, int x, int y) {
	if (coordinatesAreInsideBoard(x,y)) {
		sb_printChar(c, x, y);
	}
}

///////////////////

void printScoreboardBuffer() {
	int i;
	for (i = 0; i < SB_HEIGHT; i++) {
		sb_printString(buffer[i], 0, i);
	}
	fflush(stdout);
}

void eraseScoreboard() {
	setScoreboard();
	printScoreboardBuffer();
}

///////// PRIVATE ///////////

void sb_printChar(char c, int x, int y) {
	printf("\033[%d;%dH%c\n", y+1+SB_LOCATION_Y, x+1+SB_LOCATION_X, c);  	
	buffer[y][x] = c;
}

void sb_printStringAndWriteToBuffer(char const s[], int x , int y) {
	sb_printString(s, x ,y);
	sb_writeToBuffer(s, x , y);
}

void sb_printString(char const s[], int x , int y) {
//	printf("\033[%d;%dH%s\n", y+1+SB_LOCATION_Y, x+1+SB_LOCATION_X, s);  	
	printString(s, x+SB_LOCATION_X, y+SB_LOCATION_Y);
}

void sb_writeToBuffer(char const s[], int x , int y) {
	int i;
	for (i = 0; i < strlen(s); i++) {
		buffer[y][x+i] = s[i];
	}
}
/*
int sb_printString_noBuff(char s[], int x , int y) {
	printf("\033[%d;%dH%s\n", y+1+SB_LOCATION_Y, x+1+SB_LOCATION_X, s);  	
}
*/
