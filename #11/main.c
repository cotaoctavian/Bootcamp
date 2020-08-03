/**************************************************************
 *                      INCLUDES                              *
 **************************************************************/

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include "engine_data.h"

int main() 
{
    int response = 0;
    void *engine = initializeEngine();

    response = startEngine(engine);

    if (1 == response)
    {   
        sleep(2);

        stopEngine(engine);

        destroyEngine(engine);
    }
    
    return 0;
}