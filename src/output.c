#include <stdio.h>

#include "graphics.h"

///////// PUBLIC ////////////

int printCharXY(int c, int x, int y) {
	printf("\033[%d;%dH%c\n", y+1, x+1, c);  	
}

int printStringXY(int s[], int x, int y) {
	printf("\033[%d;%dH%s\n", y+1, x+1, s);  	
}

int printMatrixXY(char m[][8], int x, int y, int size) {
	int i;
	for (i = 0; i < size; i++) {
		printStringXY(m[i], x, y+i);
	}
}

int printMatrixXYgo(char m[][22], int x, int y, int size) {
	int i;
	for (i = 0; i < size; i++) {
		printStringXY(m[i], x, y+i);
	}
}

int printTrack() {
	printf("%s", trackNl);
}

int printTrackAdvanced() {
	int i;
	for (i = 0; i < 24; i++) {
		printf("\033[%d;%dH%s", i+1, 1, track3[i]);
	}
}

int clearScreen(void) {
	printf("\e[1;1H\e[2J");
}

//////////////////////////////

/*
POSITION getConsoleSize() {
	struct winsize w;
	ioctl(0, TIOCGWINSZ, &w);
	POSITION size;
	size.x = w.ws_col;
	size.y = w.ws_row;
	return size;
}
*/
