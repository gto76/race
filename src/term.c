#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>

#include "environment.h"
#include "graphics.h"
#include "output.h"
#include "term.h"
#include "scoreboard.h"

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
	clock_t lastKeyPress;
};
typedef struct player PLAYER;

////////////////////////////

int main(void);
OBJECT *getAllObjects(void);
int getNumberOfObjects(void);
void initPlayers(void);
void splashScreen(void);
void waitForEnter(void);
void setPlayer(PLAYER (*ppp)[], int i, int x, int y, char c, int up, int down, int right, int left);
void setStartTime(PLAYER (*ppp)[]);
void countdown(void);
void deepSleep1(void);
void clearInputBuffer(void);
int areAllFinished(PLAYER (*ppp)[]);
int weHaveAWinner(PLAYER (*ppp)[]);
void results(PLAYER (*ppp)[]);
void checkMove(char c, PLAYER (*ppp)[]);
void movePlayer(PLAYER (*ppp)[], int i, int dir);
int moveHappenedTooFast(PLAYER *p);
int isPositionValid(POSITION pos, int dir);
void saveMove(PLAYER *p, int dir);
int crossedTheLine(POSITION pos, int dir);
int getSymbolOnTheTrack(POSITION pos);
POSITION getNewPosition(POSITION pos, int dir);
void printAllPlayers(PLAYER (*ppp)[]);
void printPlayer(PLAYER *p);
void erasePlayer(PLAYER *p);
void printChar(int c, POSITION pos);
void checkeredFlag(void);
void drawFlagAndWait(char const *flag[7], int x, int y, int centiSeconds);

////////////////////////////

int const DEBUG = 0;
int const numOfPlayers = 2;

////////////////////////////

PLAYER (*players)[];

int noOfWinsP1 = 0;
int noOfWinsP2 = 0;

////////////////////////////

int const STARTING_POSITION_X = 40;
int const DENY_MOVES_FASTER_THAN = 90;

////////// MAIN ///////////

int main(void) {
	setEnvironment();
	setOutput();
	setScoreboard();

	while(1) {
		setRaceMode();
		PLAYER ppp[numOfPlayers];
		setPlayer(&ppp, 0, STARTING_POSITION_X, 18, '1', 65, 66, 67, 68);
		setPlayer(&ppp, 1, STARTING_POSITION_X, 20, '2', 119, 115, 100, 97);
		players = &ppp;

		redrawScreen();
		splashScreen();
		countdown();

		setStartTime(&ppp); 
		char c;
		while (!weHaveAWinner(&ppp)) { 
			c = getc(stdin);
			checkMove(c, &ppp);
		}
		printWins();
		checkeredFlag();
		waitForEnter();
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

int getNoOfWinsP1() {
	return noOfWinsP1;
}

int getNoOfWinsP2() {
	return noOfWinsP2;
}

int getNumberOfObjects() {
	return numOfPlayers;
}

////////////////////////////

void splashScreen() {
	int x = 1, y = 0;
	eraseScoreboard();
	printMatrixOnBoardXY(race, x, y);
	printStringOnBoardXY(" - Press Enter - ", 5, 5);
	waitForEnter();
}

void waitForEnter() {
	fflush(stdout);
	clearInputBuffer();
	char c = getc(stdin);
	while (c != '\n') {
		c = getc(stdin);
	}
}

void setPlayer(PLAYER (*ppp)[], int i, int x, int y, char c, int up, int down, int right, int left) {
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
	(*p).lastKeyPress = 0; 
}

void setStartTime(PLAYER (*ppp)[]) {
	int i;
	MOVE m;
	m.dir = 0;
	m.time = clock();
	for (i = 0; i < numOfPlayers; i++) {
		((*ppp)[i]).mmm[0] = m;
		((*ppp)[i]).lastMove++;
	}
}

void countdown() {
	int x = 10, y = 0;
	fflush(stdout);
	eraseScoreboard();
	printMatrixOnBoardXY(three, x, y);
	deepSleep1();
	printMatrixOnBoardXY(two, x, y);
	deepSleep1();
	printMatrixOnBoardXY(one, x, y);
	deepSleep1();
	clearInputBuffer();
	printMatrixOnBoardXY(go, 3, y);
}

// Goes back to sleep if awakend by signal
void deepSleep1() {
	while (sleep(1));
}

void clearInputBuffer() {
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

int weHaveAWinner(PLAYER (*ppp)[]) {
	int i;
	for (i = 0; i < numOfPlayers; i++) {
		if (((*ppp)[i]).finished == 1) {
			if (((*ppp)[i]).obj.c == '1') {
				noOfWinsP1++;
			} else if (((*ppp)[i]).obj.c == '2') {
				noOfWinsP2++;
			}
			return 1;
		}
	}
	return 0;
}

void checkeredFlag() {
	int y = 0;
	int const REPEATS = 1;
	for (int i = 0; i < REPEATS; i++) {

		drawFlagAndWait(flag[0], 5, y, 30);
		drawFlagAndWait(flag[6], 4, y, 30);
		drawFlagAndWait(flag[0], 3, y, 10);

		//drawFlagAndWait(flag[0], 3, y, 10);
		//drawFlagAndWait(flag[6], 3, y, 20);
		//drawFlagAndWait(flag[0], 3, y, 10);
		//drawFlagAndWait(flag[6], 3, y, 30);
		//drawFlagAndWait(flag[3], 3, y, 10);
	}
}

void drawFlagAndWait(char const *flag[7], int x, int y, int centiSeconds) {
	eraseScoreboard();
	printMatrixOnBoardXY(flag, x, y);
	usleep(centiSeconds*10000);	
}

void results(PLAYER (*ppp)[]) {
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

void checkMove(char c, PLAYER (*ppp)[]) {
	int i, j;
	for (i = 0; i < numOfPlayers; i++) {
		for (j = 0; j < 4; j++) {
			if ((*ppp)[i].dir[j] == c) {
				movePlayer(ppp, i, j);
				return;
			}
		}
	}
}

void movePlayer(PLAYER (*ppp)[], int i, int dir) {
	PLAYER *p = &(*ppp)[i];
	//if (moveHappenedTooFast(p)) {
	//	return;
	//}
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
		// so that it if two are on the same spot the last that arrived gets printed:
		printPlayer(p); 
	}
}

int moveHappenedTooFast(PLAYER *p) {
	clock_t now = clock();
	clock_t deltaTime = now - (*p).lastKeyPress;
	(*p).lastKeyPress = now;
	return deltaTime < DENY_MOVES_FASTER_THAN;
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

void saveMove(PLAYER *p, int dir) {
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

void printAllPlayers(PLAYER (*ppp)[]) {
	int i;
	for (i = 0; i < numOfPlayers; i++) {
		printPlayer(&((*ppp)[i])); 
	}
}

void printPlayer(PLAYER *p) {
	printCharXY((*p).obj.c, (*p).obj.pos.x, (*p).obj.pos.y);
}

void erasePlayer(PLAYER *p) {
	printCharXY(' ', (*p).obj.pos.x, (*p).obj.pos.y);
}

void printChar(int c, POSITION pos) {
	printCharXY(c, pos.x, pos.y);
}


