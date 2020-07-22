#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void handleData(void (*progressCallback)(int, int), char* data, int dataSize)
{
    int i;
    for(i = 0; data[i]; i++) {
        progressCallback(i, dataSize);
    }
}

void progressBar(int index, int dataSize)
{   
    float temp = ((float) index / (float) dataSize);
    int percent = temp * 100;
    
    int i;
    printf("%s", "[");
    for(i = 1; i <= 100; i++) {
        printf("%s", i <= percent? "#" : " ");
    }
    printf("%s %d %s\n", "]", percent, "%");
}

int main(void)
{
    char *buffer = "Testare321321321321";
    handleData(progressBar, buffer, strlen(buffer) - 1);
    
    return 0;
}