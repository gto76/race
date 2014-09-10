

/*
// 32 (space) = -1;
// 35 (#), everything else = -2
// 124 (|) = 0
int** getEmptyDistanceMatrix() {
	int paths[HEIGHT][WIDTH];
	int i, j;
	for (i = 0; i < HEIGHT; i++) {
		for (j = 0; j < WIDTH; j++) {
			char c = track[i][j];
			if (c == ' ') {
				paths[i][j] = -1;
			} else if (c == '|') {
				paths[i][j] = 0;
				paths[i][j-1] = 1;
			} else {
				paths[i][j] = -2;
			}
		}
	}
	return paths;
}

// If fields value eqals distance than assign distance+1 to all neighbouring fields
//23   323
//123 32123
//012 21012
//12   212
//12    2
//012 
//12
//2
int** getDistanceMatrix(char track[][WIDTH]) {
	int** paths = getEmptyDistanceMatrix();
	int notFinished = 1;
	int distance = 1, i, j;
	while (notFinished) {
		notFinished = 0;
		for (i = 0; i < HEIGHT; i++) {
			for (j = 0; j < WIDTH; j++) {
				if (paths[i][j] == distance) {
					notFinished = 1;
					floodNeighbours(&paths, j, i, distance);
				}
			}
		}
		distance++;
	}
	return paths;
}

void floodNeighbours(int (*paths)[][WIDTH], int x, int y, int distance) {
	setDistance(paths, x, y-1, distance);
	setDistance(paths, x, y+1, distance);
	setDistance(paths, x+1, y, distance);
	setDistance(paths, x-1, y, distance);
}

void setDistance(int (*paths)[][WIDTH], int x, int y, int distance) {
	POSITION p;
	p.x = x;
	p.y = y;
	if (isPositionLegal(p) && (*paths)[y][x] == -1) {
		(*paths)[y][x] = distance;
	}
}

int isPositionLegal(POSITION p) {
	return p.x >= 0 && p.y >= 0 && p.x < WIDTH && p.y < HEIGHT;
}
*/

