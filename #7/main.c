#include <stdio.h>
#include <dlfcn.h>
#include <stdlib.h>

/* 
    CREATE DYNAMIC LIBRARY:
        -> gcc -fPIC -shared -o sample.so solve.c

    COMPILE main.c: 
        -> gcc -o main main.c -ldl -rdynamic

    echo $LD_LIBRARY_PATH
    export LD_LIBRARY_PATH =.
    echo $LD_LIBRARY_PATH

    ldd main -> see what libraries are included.
*/

int main() 
{
    void *handler;
    int *(*allocInLib)();
    void (*freeInLib)(int *);
    char *error;

    /* Get the handler */
    handler = dlopen("sample.so", RTLD_LAZY);
    if (handler == 0)
    {
        fprintf(stderr, "%s\n", dlerror());
        return 0;
    }

    /* Clear any existing error */
    dlerror();   

    /* Get the functions */ 
    *(void **) (&allocInLib) = dlsym(handler, "allocInLib");
    *(void **) (&freeInLib) = dlsym(handler, "freeInLib");

    /* Check for errors */
    error = dlerror();
    if (NULL != error)  
    {
        fprintf(stderr, "%s\n", error);
        return 0;
    }

    /* Get the allocated variable */
    int *value = (*allocInLib)();

    /* Set value to the pointer. */
    *value = 5;

    /* Deallocate the memory */
    (*freeInLib)(value);

    /* Close the handler */
    dlclose(handler);

    return 0;
}
