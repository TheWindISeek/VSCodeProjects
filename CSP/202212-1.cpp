#include <cstdio>
#include <cmath>
using namespace std;

int main () {
    int n;
    double i;
    scanf("%d", &n);
    scanf("%lf", &i);
    double sum = 0;
    double rate[51];
    rate[0] = 1;
    for(int j = 1; j <= n; ++j) {
        rate[j] = rate[j-1] * (1.0+i);
    }

    double cur;
    for(int j = 0; j <= n; ++j) {
        scanf("%lf", &cur);
        sum += cur * rate[j];
    }

    printf("%lf", sum/rate[n]);
    return 0;
}