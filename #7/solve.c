#include <stdio.h>
#include <stdlib.h>

/* 
    7. [Linux dynamic libraries] 
    Create an library called sample.so that exports 2 functions. int* allocInLib() and void freeInLib( int* ).
    These functions allocate/deallocate an integer using malloc/free and print the operation.

    Create 2 projects. 
    First project links the .so at build time, second project loads and calls the functions in the .so
    using dlopen,dlsym,dlclose api.

    In both cases main function declares an int* loads the library if necessary,
    calls allocInLib to obtain int* , sets the value , prints it , and calls freeInLib
*/

int* allocInLib() {
    
    int* value = (int*) malloc(sizeof(int));

    return value; 
}

void freeInLib(int *value) {
    printf("The value is: %d.\nMemory deallocated.\n", *value);
    free(value);
}