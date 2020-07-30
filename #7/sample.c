/**************************************************************
 *                      INCLUDES                              *
 **************************************************************/

#include <stdio.h>
#include <stdlib.h>

/**************************************************************
 *                FUNCTIONS DECLARATION                       *
 **************************************************************/

int *alloc_in_lib();
void free_in_lib(int *value);

/**************************************************************
 *                FUNCTIONS DEFINITION                        *
 **************************************************************/

/** 
* @brief  This function allocates memory to an integer
* @return returns the allocated variable.
*/
int *alloc_in_lib() 
{   
    int *ret_val = NULL;
    int *value = (int *) malloc(sizeof(int));
    
    if (NULL != value) 
    {
        ret_val = value;
    }
    else 
    {
        ret_val = NULL;
    }

    return ret_val;
}

/**
* @brief     This function deallocates the memory of the variable
* @param[in] The variable that is going to be deallocated.
*/
void free_in_lib(int *value) 
{
    if (NULL != value)
    {
        printf("The value is: %d.\nMemory deallocated.\n", *value);
    
        free(value);
        value = NULL;
    }
    else 
    {
        printf("The value is NULL. Deallocation failed.");
    }
}
