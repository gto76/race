#include <stdio.h>
#include <sys/ioctl.h>
#include <signal.h>

#include "graphics.h"
#include "term.h"
#include "util.h"
#include "scoreboard.h"

void sigWinChCatcher(int signum);
void registerSigWinChCatcher();

int columns = TRACK_WIDTH;
int rows = TRACK_HEIGHT;

#define PRINT_IN_CENTER 1

///////// PUBLIC ////////////

int setOutput() {
	registerSigWinChCatcher();
	updateConsoleSize();
}

/////////////////////////

int printCharXY(char c, int x, int y) {
	if (coordinatesOutOfBounds(x, y))
		return;
	printf("\033[%d;%dH%c", getAbsoluteY(y), getAbsoluteX(x), c);  	
}

int printString(const char s[], int x, int y) {
	if (coordinatesOutOfBounds(x, y))
		return;
	int itDoesntFitTheScreen = strlen(s) + x > columns;
	if (itDoesntFitTheScreen) {
		int distanceToTheRightEdge = columns - x - 1;
		char subArray[distanceToTheRightEdge+2];
		copyArray(subArray, s, distanceToTheRightEdge+2);
		s = subArray;
		printf("\033[%d;%dH%s", getAbsoluteY(y), getAbsoluteX(x), subArray);
	} else {
		printf("\033[%d;%dH%s", getAbsoluteY(y), getAbsoluteX(x), s);
	}
}

int getAbsoluteX(int x) {
	return getAbsoluteCoordinate(x, columns, TRACK_WIDTH);
}

int getAbsoluteY(int y) {
	return getAbsoluteCoordinate(y, rows, TRACK_HEIGHT);
}

int getAbsoluteCoordinate(int value, int console, int track) {
	int offset = 0;
	if (PRINT_IN_CENTER) {
		offset = (console - track) / 2;
		if (offset < 0)
			offset = 0;
	}
	return value+1 + offset;
}

int coordinatesOutOfBounds(int x, int y) {
	return x >= columns || y >= rows || x < 0 || y < 0;
}


////////// DRAW ///////////

int printTrack() {
	updateConsoleSize();
	int i;
	int height;
	if (rows < 24) {
		height = rows;
	} else {
		height = 24;
	}

	for (i = 0; i < height; i++) {
		//char subArray[width+1];
		//copyArray(subArray, track[i], width+1);
		//printf("\033[%d;%dH%s", i+1, 1, subArray);
		printString(track[i], 0, i);
	}
	fflush(stdout);
}

int clearScreen(void) {
	printf("\e[1;1H\e[2J");
}

int printObject(OBJECT *obj, int i) {
	printCharXY((*obj).c, (*obj).pos.x, (*obj).pos.y);
}

int printObjects(OBJECT * ooo, int noOfObj) {
	int i;
	for (i = 0; i < noOfObj; i++) {
		printObject((ooo+i), i);
	}
}

void redrawScreen() {
	updateConsoleSize();
	clearScreen();
	printTrack();
	int noOfObj = getNumberOfObjects();
	OBJECT * ooo;
	ooo = getAllObjects();
	printObjects(ooo, noOfObj);
	printScoreboardBuffer();
	fflush(stdout);
}

/////////// SIGNALS /////////////

void registerSigWinChCatcher() {
	struct sigaction action;

	sigemptyset(&action.sa_mask);
    action.sa_flags = 0;

	action.sa_handler = sigWinChCatcher;
	sigaction(SIGWINCH, &action, NULL);
}

void sigWinChCatcher(int signum) {
	redrawScreen();
}

int updateConsoleSize() {
	struct winsize w;
	ioctl(0, TIOCGWINSZ, &w);
	columns = w.ws_col;
	rows = w.ws_row;
}

