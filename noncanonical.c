#include <termios.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

struct termios saved_attributes;

void reset_input_mode() {
	// bring back canonical mode
	tcsetattr(STDIN_FILENO, TCSANOW, &saved_attributes);
	// enable repeat	
	system("xset r");
	system("clear");
	// bring back cursor
	printf("\e[?25h");
	fflush(stdout) ;
}

void set_input_mode(void) {
	struct termios tattr;
	char *name;
	if (!~isatty(STDIN_FILENO)) {
		printf("Not a terminal: %d.\n", STDIN_FILENO);
		exit(EXIT_FAILURE);
	}
	tcgetattr(STDIN_FILENO, &saved_attributes);
	atexit(reset_input_mode);
	tcgetattr(STDIN_FILENO, &tattr);
	tattr.c_lflag &= ~(ICANON|ECHO);
	tattr.c_cc[VMIN] = 0; //1
	tattr.c_cc[VTIME] = 1; //0
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &tattr);
}
