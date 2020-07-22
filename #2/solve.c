#include <stdio.h>
#include <ctype.h>
#include <stdint.h>
#include <string.h>

/* 
 * @brief Function used to get the first number from a string
 * @param[in] - char *buffer is the string that is going to be parsed
 *            - int offset is the index from where you start searching for a number
 *            - int buffer_size is the parameter that specifies the size of the string
 * @return - returns the a number if exists in string, else is going to return an error based on a specific case
*/

uint32_t get_value(char *buffer, int offset, int buffer_size) 
{

    /* Check if the buffer is not empty. */
    if (buffer_size == 0) 
    {
        return -1;
    } 
    /* Check if the offset is not greater than buffer_size. */
    else if (offset > buffer_size) { 
        return -2;
    } 
    else 
    {   /* Search for uint32_t value in the buffer. */
        int i;
        for (i = offset; buffer[i]; i++) 
        {
            if (isdigit(buffer[i]))
            {
                return (uint32_t) buffer[i] - 48;
            }
        }

        return -3;
    }

    return 0;
}

int main() {
    char *buffer = "Tes3tare4";

    /* Check if the buffer is not NULL; */
    if (buffer != NULL) 
    {
        /* Get the function's result. */
        uint32_t result = get_value(buffer, 5, strlen(buffer) - 1);
        switch (result) 
        {
            case -1:
                printf("%s\n", "The buffer is empty.");
                break;
            case -2:
                printf("%s\n", "The offset is greater than buffer_size.");
                break;
            case -3: 
                printf("%s\n", "The buffer doesn't contain any numbers.");
                break;
            default:
                printf("%d\n", result);
                break;
        }
    }
    else
    {
        printf("%s\n", "The buffer is NULL.");
    }
    
    return 0;
}
