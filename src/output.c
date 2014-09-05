#include <stdio.h>
#include <sys/ioctl.h>
#include <signal.h>

#include "graphics.h"
#include "term.h"

void copyArray(char (*track)[], char *subArray, int width);
void sigWinChCatcher(int signum);
void registerSigWinChCatcher();

int columns = 0;
int rows = 0;

///////// PUBLIC ////////////

int setOutput() {
	registerSigWinChCatcher();
}
int printCharXY(char c, int x, int y) {
	printf("\033[%d;%dH%c\n", y+1, x+1, c);  	
}

/////////////////////////////
/*
int printTrack() {
	printf("%s", trackNl);
}
*/

int printTrack() {
	updateConsoleSize();
	int i;
	int width, height;
	if (rows < 24) {
		height = rows;
	} else {
		height = 24;
	}

	if (columns < 81) {
		width = columns; 
	} else {
		width = 81;
	}

	for (i = 0; i < height; i++) {
		char subArray[width+1];
		copyArray(&(track[i]), subArray,  width+1);
		printf("\033[%d;%dH%s", i+1, 1, subArray);
	}
}

void copyArray(char (*track)[], char *subArray, int width) {
	int i;
	for (i = 0; i < width-1; i++) {
		subArray[i] = (*track)[i];
	}
	subArray[i] = '\0';
}

int clearScreen(void) {
	printf("\e[1;1H\e[2J");
}

int printObject(OBJECT *obj, int i) {
	printCharXY((*obj).c, (*obj).pos.x, (*obj).pos.y);
	printf("\033[%d;%dH%c x:%d y:%d\n", 2+i, 2, (*obj).c, (*obj).pos.x, (*obj).pos.y);  	
}

int printObjects(OBJECT * ooo, int noOfObj) {
	int i;
	for (i = 0; i < noOfObj; i++) {
		printObject((ooo+i), i);
	}
}

//////////////////////////////

void registerSigWinChCatcher() {
	struct sigaction action;
	action.sa_handler = sigWinChCatcher;
	sigaction(SIGWINCH, &action, NULL);
}

void sigWinChCatcher(int signum) {
	updateConsoleSize();
	clearScreen();
	printTrack();
	int noOfObj = getNumberOfObjects();
	OBJECT * ooo;
	ooo = getAllObjects();
	printObjects(ooo, noOfObj);
}

//////////////////////////////

int updateConsoleSize() {
	struct winsize w;
	ioctl(0, TIOCGWINSZ, &w);
	columns = w.ws_col;
	rows = w.ws_row;
}
