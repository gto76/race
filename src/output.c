#include <stdio.h>
#include <sys/ioctl.h>
#include <signal.h>

#include "graphics.h"
#include "term.h"
#include "util.h"
#include "scoreboard.h"

void sigWinChCatcher(int signum);
void registerSigWinChCatcher();

int columns = 80;
int rows = 24;

///////// PUBLIC ////////////

int setOutput() {
	registerSigWinChCatcher();
	updateConsoleSize();
}

int printCharXY(char c, int x, int y) {
	if (x < columns && y < rows) {
		printf("\033[%d;%dH%c", y+1, x+1, c);  	
	}
}

////////// DRAW ///////////
/* OLD
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
		//copyArray(&(track[i]), subArray,  width+1); OLD
		copyArray(&(track[i]), &subArray,  width+1); 

		printf("\033[%d;%dH%s", i+1, 1, subArray);
	}
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

