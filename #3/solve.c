#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/**
* @brief     - The function must print each decimal value of each byte contained in char *data.
* @param[in] - *progress_callback - is used to call the function progress_callback
*            - *data - is the string that is going to be parsed
*            - data_size - is the size of the string
*/

void handle_data (void (*progress_callback)(int, int), char *data, int data_size)
{
    if (data != NULL) 
    {
        int i = 0;

        for (i = 0; data[i]; i++) 
        {
            progress_callback(i, data_size);
        }
    } 
    else 
    {
        printf("%s\n", "The buffer is NULL.");
    }
}

/** 
* @brief     - The function is used to print out the progress bar
* @param[in] - index - is the index of the string
*            - data_size - is the size of the string
*/
void progress_bar (int index, int data_size)
{   
    if (data_size != 0) 
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
        printf("%s\n", "Data size can't be zero.");
    }
}

int main()
{
    char *buffer = (char *) malloc (19);

    if (buffer != NULL) 
    {
        strcpy(buffer, "Testare321321321321"); 
        handle_data(progress_bar, buffer, strlen(buffer) - 1);
    }

    return 0;
}
