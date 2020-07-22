#include <stdio.h>
#include <stdint.h>

/* 
    * @brief Converting from base 10 to base 2.
    * @param[in] uint32_t value - integer in base 10
*/

void print_bits(uint32_t value)
{
    for (unsigned bit = 1u << 31; bit != 0; bit >>= 1) 
    {
        printf("%c", (value & bit) ? '1' : '0');
    }

    printf("\n");
}

int main() 
{ 
    print_bits(16);
    return 0;
}
