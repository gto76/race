void copyArray(char dest[], const char src[], int width) {
	int i;
	for (i = 0; i < width-1; i++) {
		dest[i] = src[i];
	}
	dest[width-1] = '\0';
}
