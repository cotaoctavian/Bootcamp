#include <stdio.h>
#include <ctype.h>
#include <stdint.h>
#include <string.h>

/** 
 * @brief         - Function used to get the first number from a string
 * @param[in]     - *buffer - is the string that is going to be parsed
 *                - offset - is the index from where you start searching for a number
 *                - buffer_size - is the parameter that specifies the size of the string
 * @param[in/out] - result - is the converted integer from the buffer's bytes.
 * @return        - returns the a number if exists in string, else is going to return an error based on a specific case
*/
uint32_t get_value(char *buffer, int offset, int buffer_size, uint32_t *result) 
{   
    int ret_val = 0;

    if (NULL == buffer) 
    {
        printf("The buffer is null.");
        ret_val = -1;
    } 
    else 
    {
        if (offset > buffer_size - 4) 
        {
            printf("Offset is greater than maximum limit.\n");
            ret_val = -2;
        } 
        else 
        { 
            *result = *(uint32_t *) (buffer + offset);
        }
    }

    return ret_val;
}

int main() 
{ 
    char buffer[] = {0x01, 0x11, 0x11, 0x00, 0x00, 0x00, 0x01}; 
    uint32_t result = 0;
    int response = get_value(buffer, 2, 7, &result);

    if (0 == response) 
    {
        printf("%d\n", result);
    } 
    else 
    {
        printf("Something went wrong while trying to convert the buffer to integer.\n");
    }

    return 0; 
} 
