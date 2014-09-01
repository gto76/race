#include <termios.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

struct termios saved_attributes;

void setEnvironment();
void setInputMode(void);
void registerSigIntCatcher();
void sigIntCatcher(int signum);
void disableRepeatAndCursor();
void resetEnvironment();
void resetInputMode();
void enableRepeatAndCursor();


//////// AT START ////////////

void setEnvironment() {
	setInputMode();
	registerSigIntCatcher();
	disableRepeatAndCursor();
}

void setInputMode(void) {
	struct termios tattr;
	char *name;
	if (!~isatty(STDIN_FILENO)) {
		printf("Not a terminal: %d.\n", STDIN_FILENO);
		exit(EXIT_FAILURE);
	}
	// set noncanonical mode, disable echo, set to nonblocking
	tcgetattr(STDIN_FILENO, &saved_attributes);
	atexit(resetEnvironment);
	tcgetattr(STDIN_FILENO, &tattr);
	tattr.c_lflag &= ~(ICANON|ECHO);
	tattr.c_cc[VMIN] = 0; //1
	tattr.c_cc[VTIME] = 1; //0
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &tattr);
}

void registerSigIntCatcher() {
	struct sigaction action;
	action.sa_handler = sigIntCatcher;
	sigaction(SIGINT, &action, NULL);
}

// method that gets executed when ctrl-c is pressed.
// necesary so that at_exit method gets executed,
// that sets terminal back to the original state.
void sigIntCatcher(int signum) {
	exit(0);
}

void disableRepeatAndCursor() {
	// disable repeat
	system("xset -r"); 
	// set cursor off. could also probably use system("setterm -cursor off);
	printf("\e[?25l");
	fflush(stdout);
}

///////// AT END ////////////

void resetEnvironment() {
	resetInputMode();
	enableRepeatAndCursor();
}

void resetInputMode() {
	// bring back canonical mode
	tcsetattr(STDIN_FILENO, TCSANOW, &saved_attributes);
}

void enableRepeatAndCursor() {
	// enable repeat	
	system("xset r");
	system("clear");
	// bring back cursor
	printf("\e[?25h");
	fflush(stdout) ;
}

