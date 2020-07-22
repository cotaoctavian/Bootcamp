#include <stdio.h>

#define TM_PRINTF(debugLevel, f_, ...) printf("%d %s %d\n", debugLevel, __FILE__, __LINE__); printf((f_), __VA_ARGS__)

int main() {
    TM_PRINTF(5, "My value is %d\n", 5);
    return 0;
}