/**************************************************************
 *                      INCLUDES                              *
 **************************************************************/

#include <stdio.h>

/**************************************************************
 *                      DEFINES                               *
 **************************************************************/

/**    
* @brief     - This macro prints out information about the file, the line and the formatted text
* @param[in] - debug_level - is an integer from 1 to 5
* @param[in] - f_          - is the format of the text
* @param[in] - ...         - stores one or more parameters of everything you want to print out
*/
#define TM_PRINTF(debug_level, f_, ...) \
    printf("%d %s %d\n", debug_level, __FILE__, __LINE__); \
    printf((f_), __VA_ARGS__)
    
int main() 
{
    TM_PRINTF(5, "My value is %d\n", 5);
    return 0;
}
