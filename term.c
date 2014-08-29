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
};
typedef struct player PLAYER;

int checkMove(char c, PLAYER (*p)[]);
int checkMove(char c, PLAYER (*p)[]);
void term(int signum);
int main(void);
int clearScreen(void);
int printPlayer(PLAYER *p);
int erasePlayer(PLAYER *p);
int isPositionValid(POSITION pos);
int movePlayer(PLAYER *p, int dir);
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
"#                                                                              #\n",
"#                                                                              #\n",
"#                                                                              #\n",
"#                                                                              #\n",
"#                                                                              #\n",
"#                                                                              #\n",
"#                                                                              #\n",
"################################################################################",
};


int const numOfPlayers = 1;


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

	PLAYER p[numOfPlayers];
	p[0].pos.x = 10;
	p[0].pos.y = 10;
	p[0].c = '1';
	p[0].dir[0] = 65; // up
	p[0].dir[1] = 66; // down
	p[0].dir[2] = 67; // right
	p[0].dir[3] = 68; // left

	printf("%s", track);

	char c;
	while(1) {
		c = getc(stdin);
		checkMove(c, &p);
		printPlayer(&p[0]); 
	}

	return EXIT_SUCCESS;
}	

int checkMove(char c, PLAYER (*p)[]) {
	int i, j;
	for (i = 0; i < numOfPlayers; i++) {
		for (j = 0; j < 4; j++) {
			if ((*p)[i].dir[j] == c) {
				movePlayer(&((*p)[i]), j);
				return 0;
			}
		}
	}
}

int movePlayer(PLAYER *p, int dir) {
	POSITION newPosition = getNewPosition((*p).pos, dir);
	if (isPositionValid(newPosition)) {
		erasePlayer(p);
		(*p).pos = newPosition;
		printPlayer(p);
	}
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

int isPositionValid(POSITION pos) {
	return track[pos.y][pos.x] == ' ';
}

int erasePlayer(PLAYER *p) {
	printf("\033[%d;%dH%c\n", (*p).pos.y+1, (*p).pos.x+1, ' ');
}

int printPlayer(PLAYER *p) {
	printf("\033[%d;%dH%c\n", (*p).pos.y+1, (*p).pos.x+1, (*p).c);
}

int clearScreen(void) {
	printf("\e[1;1H\e[2J");
}

// method that gets executed when ctrl-c is pressed
void term(int signum) {
	exit(0);
}
