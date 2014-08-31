#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#include "noncanonical.h"

int const DEBUG = 0;

enum direction {UP, DOWN, RIGHT, LEFT};

struct position {
	int x, y;
};
typedef struct position POSITION;

struct player {
	POSITION pos;
	char c;
	int dir[4]; // charcode of all directions
	int finished;
};
typedef struct player PLAYER;

int checkMove(char c, PLAYER (*p)[]);
int checkMove(char c, PLAYER (*p)[]);
void term(int signum);
int main(void);
int clearScreen(void);
int printPlayer(PLAYER *p);
int erasePlayer(PLAYER *p);
int isPositionValid(POSITION pos, int dir);
int movePlayer(PLAYER (*p)[], int i, int dir);
POSITION getNewPosition(POSITION pos, int dir);

char const track[][81] = {
"################################################################################\n",
"#                                                                              #\n",
"#                                                                              #\n",
"#                                                                              #\n",
"#                                                                              #\n",
"#                                                                              #\n",
"#                                                                              #\n",
"#                                                                              #\n",
"#                  ##########################################                  #\n",
"#                  ##########################################                  #\n",
"#                  ########                          ########                  #\n",
"#                  ##   ###                          ###   ##                  #\n",
"#                  ##   ###                          ###   ##                  #\n",
"#                  ##   ###                          ###   ##                  #\n",
"#                  ##########################################                  #\n",
"#                  ##########################################                  #\n",
"#                                      |                                       #\n",
"#                                      |                                       #\n",
"#                                      |                                       #\n",
"#                                      |                                       #\n",
"#                                      |                                       #\n",
"#                                      |                                       #\n",
"#                                      |                                       #\n",
"################################################################################",
};


int const numOfPlayers = 2;

int main(void) {
	// set terminal in noncanonical mode
	set_input_mode();
	clearScreen();

	// catch sigterm
	struct sigaction action;
	action.sa_handler = term;
	sigaction(SIGINT, &action, NULL);

	// disable repeat
	system("xset -r"); 

	PLAYER ppp[numOfPlayers];
	ppp[0].pos.x = 30; // 40
	ppp[0].pos.y = 18;
	ppp[0].c = '1';
	ppp[0].dir[0] = 65; // up
	ppp[0].dir[1] = 66; // down
	ppp[0].dir[2] = 67; // right
	ppp[0].dir[3] = 68; // left
	ppp[0].finished = 0; 

	ppp[1].pos.x = 30;
	ppp[1].pos.y = 20;
	ppp[1].c = '2';
	ppp[1].dir[0] = 119; // up - w
	ppp[1].dir[1] = 115; // down - s
	ppp[1].dir[2] = 100; // right - d
	ppp[1].dir[3] = 97; // left - a
	ppp[1].finished = 0; 

	printf("%s", track);
	printAllPlayers(&ppp);

	char c;
	while(!areAllFinished(&ppp)) {
		c = getc(stdin);
		checkMove(c, &ppp);
	}

	return EXIT_SUCCESS;
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

int printAllPlayers(PLAYER (*ppp)[]) {
	int i;
	for (i = 0; i < numOfPlayers; i++) {
		printPlayer(&((*ppp)[i])); 
	}
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

int movePlayer(PLAYER (*ppp)[], int i, int dir) { //PLAYER *p, int dir) {
		//		movePlayer(&((*p)[i]), j);
	PLAYER *p = &(*ppp)[i];
	POSITION oldPosition = (*p).pos;
	POSITION newPosition = getNewPosition(oldPosition, dir);
	if (isPositionValid(newPosition, dir)) {
		int symbol = getSymbolOnTheTrack(oldPosition);
		erasePlayer(p); //p
		if (symbol == '|') { // if player was on the finish line, draw finish line
			printChar('|', oldPosition);
		}
		if (crossedTheLine(oldPosition, dir)) { // if player crossed the line, finished flag is set to true
			(*p).finished = 1;
		}
		(*p).pos = newPosition;
		printAllPlayers(ppp); // so that if two were on the same spot both get printed
		printPlayer(p); // so that it if two are on the same spot the last thet arrived gets printed
	}
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

int isPositionValid(POSITION pos, int dir) {
	if (track[pos.y][pos.x] == ' ') {
		return 1;
	}
	if (track[pos.y][pos.x] == '|' && dir == RIGHT) {
		return 1;
	}
	return 0;
}

int erasePlayer(PLAYER *p) {
	printf("\033[%d;%dH%c\n", (*p).pos.y+1, (*p).pos.x+1, ' ');
}

int printPlayer(PLAYER *p) {
	printf("\033[%d;%dH%c\n", (*p).pos.y+1, (*p).pos.x+1, (*p).c);
}

int printChar(int c, POSITION pos) {
	printf("\033[%d;%dH%c\n", pos.y+1, pos.x+1, c);
}

int clearScreen(void) {
	printf("\e[1;1H\e[2J");
}

// method that gets executed when ctrl-c is pressed
void term(int signum) {
	exit(0);
}
