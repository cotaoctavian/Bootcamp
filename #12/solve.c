#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAP_SIZE 500

uint32_t pos = 0;

typedef struct _Map
{
    char *key;
    int value;  
} Map;

/*
    * @brief - Search for a key, if it's not found, then add it on the map, else increment the value of it.
    * @param[in] - Map map[] stores all the pairs of the type <key, value>
    *            - char *key is the key that we are looking for in the map
*/
void addKey(Map map[], char *key) 
{
    int i;
    int found = 0;
    for (i = 0; i < MAP_SIZE; i++) 
    {
        if (strcmp(map[i].key, key) == 0) 
        {
            found = 1;
            map[i].value++;
        }
    }

    if (!found) 
    {
        map[pos].key = key;
        map[pos].value = 1;
        pos++;
    }
}

/*
    * @brief - Sorts the items from the map based on the value of the keys.
    * @param[in] - Map map[] is the map that is going to be sorted
*/
void sortItemsByApparitions(Map map[])
{
    int i, j;
    for (i = 0; i < pos - 1; i++) 
    {
        for (j = i + 1; j < pos; j++) 
        {
            if (map[i].value < map[j].value) 
            {
                Map temp = map[i];

                map[i].key = map[j].key;
                map[i].value = map[j].value;

                map[j].key = temp.key;
                map[j].value = temp.value;
            } 
        }
    }
}

/* 
    * @brief - This functions prints out all the pairs from the map
*/
void printItems(Map map[])
{
    int i;
    for (i = 0; i < pos; i++)
    {
        printf("Apparitions: %d ---> Word: %s\n", map[i].value, map[i].key);
    }
}


int main() 
{
    Map map[MAP_SIZE];
    
    int i;
    /* Initialize map with default values. */
    for(i = 0; i < MAP_SIZE; i++) 
    {
        map[i].key = "";
        map[i].value = 0;
    }

    /* Declare a file pointer */
    FILE *infile;
    char *buffer;
    long numbytes;
    
    /* Open an existing file for reading */
    infile = fopen("#12/test.txt", "r");
    
    /* Quit if the file does not exist */
    if (infile == NULL)
        return 1;
    
    /* Get the number of bytes */
    fseek(infile, 0L, SEEK_END);
    numbytes = ftell(infile);
    
    /* Reset the file position indicator to the beginning of the file */
    fseek(infile, 0L, SEEK_SET);	
    
    /* Allocate sufficient memory for the buffer to hold the text */
    buffer = (char *)calloc(numbytes, sizeof(char));	
    
    /* Memory error */
    if (buffer == NULL)
        return 1;
    
    /* Copy all the text into the buffer */
    fread(buffer, sizeof(char), numbytes, infile);
    fclose(infile);

    printf("%s\n", buffer);
    
    /* Separators */
    const char s[] = "-,.?!;: ";
    char *token;
    
    token = strtok(buffer, s);
    
    /* Parsing through the string to get all the distinct words and count their apparitions */
    while (token != NULL) 
    {
        //printf("%s\n", token);
        addKey(map, token);
        token = strtok(NULL, s);
    }

    sortItemsByApparitions(map);
    printItems(map);

    /* Free the memory we used for the buffer */
    free(buffer);

    return 0;
}
