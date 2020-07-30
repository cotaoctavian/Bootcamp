/**************************************************************
 *                      INCLUDES                              *
 **************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/**************************************************************
 *                FUNCTIONS DECLARATION                       *
 **************************************************************/

static void handle_data (void (*progress_callback)(int, int), char *data, int data_size);
static void progress_bar (int index, int data_size);

/**************************************************************
 *                FUNCTIONS DEFINITION                        *
 **************************************************************/

/**
* @brief     - The function must print each decimal value of each byte contained in char *data.
* @param[in] - progress_callback - is used to call the function progress_callback
* @param[in] - data              - is the string that is going to be parsed
* @param[in] - data_size         - is the size of the string
*/
static void handle_data (void (*progress_callback)(int, int), char *data, int data_size)
{
    if (NULL != data) 
    {
        int i = 0;

        for (i = 0; data[i]; i++) 
        {
            progress_callback(i, data_size);
        }
    } 
    else 
    {
        printf("The buffer is NULL.\n");
    }
}

/** 
* @brief     - The function is used to print out the progress bar
* @param[in] - index     - is the index of the string
* @param[in] - data_size - is the size of the string
*/
static void progress_bar (int index, int data_size)
{   
    if (0 != data_size) 
    {
        float temp = 0.0f;
        int percent = 0;

        temp = ((float) index / (float) data_size);
        percent = temp * 100;
        
        int i = 0;
        printf("%s", "[");

        for (i = 1; i <= 100; i++) 
        {
            printf("%s", i <= percent? "#" : " ");
        }

        printf("%s %d %s\n", "]", percent, "%");
    } 
    else 
    { 
        printf("Data size can't be zero.\n");
    }
}

int main()
{
    char buffer[256] = {0};

    scanf("%s", buffer);

    if (NULL != buffer || strlen(buffer) == 0) 
    {
        handle_data(progress_bar, buffer, strlen(buffer) - 1);
    }
    else 
    {
        printf("The buffer is NULL.");
    }

    return 0;
}
