#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* 
    * @brief - The function must print each decimal value of each byte contained in char *data.
    * @param[in] - void (*progress_callback) is used to call the function progress_callback
    *            - char *data is the string that is going to be parsed
    *            - int data_size is the size of the string
*/

void handle_data (void (*progress_callback)(int, int), char *data, int data_size)
{
    int i;
    for (i = 0; data[i]; i++) 
    {
        progress_callback(i, data_size);
    }
}

/* 
    * @brief - The function is used to print out the progress bar
    * @param[in] - int index is the index of the string
    *            - int data_size is the size of the string
*/ 

void progress_bar (int index, int data_size)
{   
    float temp = ((float) index / (float) data_size);
    int percent = temp * 100;
    
    int i;
    printf("%s", "[");

    for (i = 1; i <= 100; i++) 
    {
        printf("%s", i <= percent? "#" : " ");
    }

    printf("%s %d %s\n", "]", percent, "%");
}

int main()
{
    char *buffer = "Testare321321321321";

    handle_data(progress_bar, buffer, strlen(buffer) - 1);
    
    return 0;
}
