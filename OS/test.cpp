#include <stdio.h>
#include <stdlib.h>

int isLessOrEqual(int x, int y) {
	int z = (y + ~x + 1) >> 31;
	int a = x >> 31;
	int b = y >> 31;
	int axorb = a ^ b;
	int result = (~axorb& z) | (axorb & b);
	return !result;
}


int main () {
    
    return 0;
}