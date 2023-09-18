#include <stdio.h>

void stack(int bss, int *data) {
    int a;
    int b;
    int c = 9;
    int *p = &a;
    printf("stack function:\n");
    printf("a:\t%p\nb:\t%p\nc:\t%p\np:\t%p\n", &a, &b, &c, &p);
    int ip[2];
    int cs[4];
    ip[0] = *(++p);
    ip[1] = *(++p);
    cs[0] = *(++p);
    cs[1] = *(++p);
    cs[2] = *(++p);
    cs[3] = *(++p);
    printf("\n\nip:\t%08X %08X\n", ip[1], ip[0]);
    printf("cs:\t%08X %08X %08X %08X\n", cs[3], cs[2], cs[1], cs[0]);
    printf("\np->\t%p\nbss:\t%p\ndata:\t%p\n", p, &bss, &data);
    printf("%d\n%d\n%d\n", a, b, c);
}

int main () {
    int bss;
    int data = 0;
    int *p = &data;
    *(p++) = 2;
    printf("%ld\np->\t%p\n", sizeof(p), p);
    printf("bss:\t%p\ndata:\t%p\np:\t%p\n", &bss, &data, &p);
    stack(bss, &data);
    printf("%d\n%d\n", bss, data);
    return 0;
}