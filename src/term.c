#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "environment.h"
#include "graphics.h"
#include "output.h"
#include "term.h"

////////////////////////////

/*
// term.h:
struct position {
	int x, y;
};
typedef struct position POSITION;

struct object {
	POSITION pos;
	char c;
}
typedef struct object OBJECT;
*/

int const WIDTH = 81;
int const HEIGHT = 24;

enum direction {UP, DOWN, RIGHT, LEFT};

struct move {
	int dir;
	clock_t time;
};
typedef struct move MOVE;

struct player {
	OBJECT obj; // position and char
	int dir[4]; // keys for all directions
	int finished; 
	MOVE mmm[1000]; // move log
	int lastMove;
};
typedef struct player PLAYER;

////////////////////////////

int checkMove(char c, PLAYER (*p)[]);
int checkMove(char c, PLAYER (*p)[]);
int main(void);
int clearScreen(void);
int printPlayer(PLAYER *p);
int erasePlayer(PLAYER *p);
int isPositionValid(POSITION pos, int dir);
int movePlayer(PLAYER (*p)[], int i, int dir);
POSITION getNewPosition(POSITION pos, int dir);
int setPlayer(PLAYER (*ppp)[], int i, int x, int y, char c, int up, int down, int right, int left);


void setDistance(int (*paths)[][WIDTH], int x, int y, int distance);

////////////////////////////

int const DEBUG = 0;
int const numOfPlayers = 2;

////////////////////////////

PLAYER (*players)[];

////////// MAIN ///////////

int main(void) {
	setEnvironment();
	setOutput();

	while (1) { // outer game loop (menu -> game...)
		setRaceMode();
		char c;
		PLAYER ppp[numOfPlayers];

		setPlayer(&ppp, 0, 30, 18, '1', 65, 66, 67, 68);
		setPlayer(&ppp, 1, 30, 20, '2', 119, 115, 100, 97);

		players = &ppp;

		//updateConsoleSize();
		//clearScreen();
		//printTrack();
		//printAllPlayers(&ppp);
		redrawScreen();
		fflush(stdout);

		countdown();
		setStartTime(&ppp);

		while (!areAllFinished(&ppp)) { // inner game loop - race
			c = getc(stdin);
			checkMove(c, &ppp);
		}
		results(&ppp);
	}
	return EXIT_SUCCESS;
}	

//////// PUBLIC /////////////

OBJECT * getAllObjects() {
	static OBJECT ooo[10];
	int i;
	for (i = 0; i < numOfPlayers; i++) {
		ooo[i] = (*players)[i].obj;
	}
	return ooo;
}

int getNumberOfObjects() {
	return numOfPlayers;
}

////////////////////////////

int setPlayer(PLAYER (*ppp)[], int i, int x, int y, char c, int up, int down, int right, int left) {
	PLAYER *p = &((*ppp)[i]);
	(*p).obj.pos.x = x;
	(*p).obj.pos.y = y;
	(*p).obj.c = c;
	(*p).dir[0] = up; 
	(*p).dir[1] = down;
	(*p).dir[2] = right;
	(*p).dir[3] = left;
	(*p).finished = 0; 
	(*p).lastMove = 0; 
}

int setStartTime(PLAYER (*ppp)[]) {
	int i;
	MOVE m;
	m.dir = 0;
	m.time = clock();
	for (i = 0; i < numOfPlayers; i++) {
		((*ppp)[i]).mmm[0] = m;
		((*ppp)[i]).lastMove++;
	}
}

int countdown() {
	int sec = 1;
	int x = 36, y = 9;
	//printMatrixOnBoardXY(three, x, y, 5);
	sleep(sec);
	//printMatrixOnBoardXY(two, x, y, 5);
	sleep(sec);
	//printMatrixOnBoardXY(one, x, y, 5);
	sleep(sec);
	clearInputBuffer();
	//printMatrixOnBoardXYgo(go, 29, y, 5);
}

int clearInputBuffer() {
	int const COUNTDOWN_BUFFER = 120; 
	char buf[COUNTDOWN_BUFFER * 3]; // you need 3 chars to clear one keystroke
	size_t nbytes;
	nbytes = sizeof(buf);
	read(0, buf, nbytes);
}

int areAllFinished(PLAYER (*ppp)[]) {
	int i;
	for (i = 0; i < numOfPlayers; i++) {
		if (((*ppp)[i]).finished == 0) {
			return 0;
		}
	}
	return 1;
}

int results(PLAYER (*ppp)[]) {
	int i;
	for (i = 0; i < numOfPlayers; i++) {
		PLAYER *p = &((*ppp)[i]);
		clock_t start = (*p).mmm[0].time;
		clock_t end = (*p).mmm[(*p).lastMove-1].time;
		double cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\033[%d;%dH%c: %f\n", 10+i, 30, (*p).obj.c, cpu_time_used);  	
	}
	sleep(2);
	clearInputBuffer();
	setMenuMode();
}

int checkMove(char c, PLAYER (*ppp)[]) {
	int i, j;
	for (i = 0; i < numOfPlayers; i++) {
		for (j = 0; j < 4; j++) {
			if ((*ppp)[i].dir[j] == c) {
				movePlayer(ppp, i, j);
				return 0;
			}
		}
	}
}

int movePlayer(PLAYER (*ppp)[], int i, int dir) {
	PLAYER *p = &(*ppp)[i];
	POSITION oldPosition = (*p).obj.pos;
	POSITION newPosition = getNewPosition(oldPosition, dir);
	if (isPositionValid(newPosition, dir)) {
		// save move if not yet finished:
		if ((*p).finished != 1) {
			saveMove(p, dir);
		}
		// if player crossed the line, finished flag is set to true:
		if (crossedTheLine(oldPosition, dir)) {
			(*p).finished = 1;
		}
		int symbol = getSymbolOnTheTrack(oldPosition);
		erasePlayer(p); 
		// if player was on the finish line, draw finish line:
		if (symbol == '|') { 
			printChar('|', oldPosition);
		}
		(*p).obj.pos = newPosition;
		// so that if two were on the same spot both get printed:
		printAllPlayers(ppp);
		// so that it if two are on the same spot the last thet arrived gets printed:
		printPlayer(p); 
	}
}

int isPositionValid(POSITION pos, int dir) {
	if (track[pos.y][pos.x] == ' ') {
		return 1;
	}
	if (track[pos.y][pos.x] == '|' && dir != LEFT) {
		return 1;
	}
	return 0;
}

int saveMove(PLAYER *p, int dir) {
	MOVE m;
	m.dir = dir;
	m.time = clock();
	(*p).mmm[(*p).lastMove++] = m;
}

int crossedTheLine(POSITION pos, int dir) {
	return getSymbolOnTheTrack(pos) == '|' && dir == RIGHT;
}

int getSymbolOnTheTrack(POSITION pos) {
	return track[pos.y][pos.x];
}

POSITION getNewPosition(POSITION pos, int dir) {
	switch (dir) {
		case UP:
			pos.y--;
			break;
		case DOWN:
			pos.y++;
			break;
		case RIGHT:
			pos.x++;
			break;
		case LEFT:
			pos.x--;
			break;
	}
	return pos;
}

	  ///////////////
	 //// print ////
	///////////////

int printAllPlayers(PLAYER (*ppp)[]) {
	int i;
	for (i = 0; i < numOfPlayers; i++) {
		printPlayer(&((*ppp)[i])); 
	}
}

int printPlayer(PLAYER *p) {
	printCharXY((*p).obj.c, (*p).obj.pos.x, (*p).obj.pos.y);
}

int erasePlayer(PLAYER *p) {
	printCharXY(' ', (*p).obj.pos.x, (*p).obj.pos.y);
}

int printChar(int c, POSITION pos) {
	printCharXY(c, pos.x, pos.y);
}


