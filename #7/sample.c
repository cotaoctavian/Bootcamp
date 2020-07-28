#include <stdio.h>
#include <stdlib.h>

/** 
* @brief  - This function allocates memory to an integer
* @return - returns the allocated variable.
*/
int *alloc_in_lib() 
{
    int *value = (int *) malloc(sizeof(int));

    return value; 
}

/**
* @brief     - This function deallocates the memory of the variable
* @param[in] - The variable that is going to be deallocated.
*/
void free_in_lib(int *value) 
{
    printf("The value is: %d.\nMemory deallocated.\n", *value);
    
    free(value);
}
