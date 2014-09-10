/*
void copyArray(const char (*track)[], char *subArray, int width) {
	int i;
	for (i = 0; i < width-2; i++) {
		subArray[i] = (*track)[i];
	}
	subArray[width-1] = '\0';
}
*/

void copyArray(char (*array)[], char (*subArray)[], int width) {
	int i;
	for (i = 0; i < width; i++) {
		(*subArray)[i] = (*array)[i];
	}
	(*subArray)[width] = '\0';
}
