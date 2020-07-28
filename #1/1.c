#include <stdio.h>
#include <stdint.h>

/**
* @brief     - Converting from base 10 to base 2.
* @param[in] - value - is integer in base 10
*/

void print_bits(uint32_t value)
{

    unsigned int bit = 1u << 31;

    while (0 != bit) 
    {
        printf("%c", (value & bit) ? '1' : '0');
        bit >>= 1;
    }

    printf("\n");
}

int main() 
{ 
    print_bits(16);
    return 0;
}
