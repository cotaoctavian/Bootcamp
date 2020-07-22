#include <stdio.h>
#include <stdint.h>

void printBits(uint32_t value) {
    for (unsigned bit = 1u << 31; bit != 0; bit >>= 1){
        putchar((value & bit) ? '1' : '0');
    }

    printf("\n");
}

int main(){ 
    printBits(16);
    return 0;
}
