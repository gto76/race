struct position {
	int x, y;
};
typedef struct position POSITION;

struct object {
	POSITION pos;
	char c;
};
typedef struct object OBJECT;

OBJECT * getAllObjects();
int getNumberOfObjects();
int getNoOfWinsP1();
int getNoOfWinsP2();
