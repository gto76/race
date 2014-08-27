#include<stdio.h>
#include<termios.h>
#include<unistd.h>
#include<stdlib.h>

struct termios saved_attributes;

void reset_input_mode() {
	tcsetattr(STDIN_FILENO, TCSANOW, &saved_attributes);
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
	tattr.c_cc[VMIN] = 1;
	tattr.c_cc[VTIME] = 0;
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &tattr);
}

int main(void) {
	char c;
	printf("Setting input mode.\n");
	set_input_mode();
	printf("Input mode set.\n");

	while(1) {
		read(STDIN_FILENO, &c, 1);
		if (c == '\004') {
			break;
		} else if (c == 't') {
			break;	
		} else {
			putchar(c);
		}
	}
	return EXIT_SUCCESS;
}	

