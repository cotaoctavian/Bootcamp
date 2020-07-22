#include <stdio.h>
#include <stdlib.h>

/* 
    7. [Linux dynamic libraries] 
    Create an library called sample.so that exports 2 functions. int* alloc_in_lib() and void free_in_lib( int* ).
    These functions allocate/deallocate an integer using malloc/free and print the operation.

    Create 2 projects. 
    First project links the .so at build time, second project loads and calls the functions in the .so
    using dlopen,dlsym,dlclose api.

    In both cases main function declares an int* loads the library if necessary,
    calls alloc_in_lib to obtain int* , sets the value , prints it , and calls free_in_lib
*/


/* 
    * @brief - This function allocates memory to an integer
    * @return - returns the allocated variable.
*/
int *alloc_in_lib() 
{
    
    int *value = (int *) malloc(sizeof(int));

    return value; 
}

/*
    * @brief - This function deallocates the memory of the variable
    * @param[in] - The variable that is going to be deallocated.
*/
void free_in_lib(int *value) 
{
    printf("The value is: %d.\nMemory deallocated.\n", *value);
    
    free(value);
}
