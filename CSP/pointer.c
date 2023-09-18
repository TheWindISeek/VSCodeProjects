#include <stdio.h>
/*
test1
    1 2 3
    1 2 3
test2
    1 2 1
    1 2 3
*/
int main () {
    int a[3];
    for(int i = 1; i <= 3; ++i) {
        scanf("%d", a+i);
    }

    for(int i = 1; i <= 3; ++i) {
        printf("%d ", *(a+i));
    }
    return 0;
}