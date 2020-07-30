/**************************************************************
 *                      INCLUDES                              *
 **************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**************************************************************
 *                      DEFINES                               *
 **************************************************************/

#define MAP_SIZE 5000

typedef struct _Map
{
    char key[256];
    int value;  
} Map;

/**************************************************************
 *                 GLOBAL VARIABLES                           *
 **************************************************************/

static uint32_t size = 0;

/**************************************************************
 *                FUNCTIONS DECLARATION                       *
 **************************************************************/

static void add_key(Map map[], char *key);
static void sort_items_by_apparitions(Map map[]);
static void print_items(Map map[]);

/**************************************************************
 *                FUNCTIONS DEFINITION                        *
 **************************************************************/

/**
* @brief     Search for a key, if it's not found, then add it on the map, else increment the value of it.
* @param[in] map - stores all the pairs of the type <key, value>
* @param[in] key - is the key that we are looking for in the map
*/
static void add_key(Map map[], char key[]) 
{
    int i = 0;
    int found = 0;

    for (i = 0; i < MAP_SIZE; i++) 
    {
        if (0 == strcmp(map[i].key, key)) 
        {
            found = 1;
            map[i].value++;
            break;
        }
    }

    if (0 == found) 
    {
        strcpy(map[size].key, key);
        map[size].value = 1;
        size++;
    }
}

/**
* @brief     Sorts the items from the map based on the value of the keys.
* @param[in] map - is the map that is going to be sorted
*/
static void sort_items_by_apparitions(Map map[])
{
    int i = 0;
    int j = 0;

    for (i = 0; i < size - 1; i++) 
    {
        for (j = i + 1; j < size; j++) 
        {
            if (map[i].value < map[j].value) 
            {
                Map temp = map[i];

                strcpy(map[i].key, map[j].key);
                map[i].value = map[j].value;

                strcpy(map[j].key, temp.key);
                map[j].value = temp.value;
            } 
        }
    }
}

/** 
* @brief     This functions prints out all the pairs from the map
* @param[in] map - is the data structure containing all the words, sorted base on their apparitions.
*/
static void print_items(Map map[])
{
    int i = 0;

    for (i = 0; i < size; i++)
    {
        printf("Apparitions: %d ---> Word: %s\n", map[i].value, map[i].key);
    }
}

int main() 
{
    Map map[MAP_SIZE];
    FILE *file;
    int i = 0;

    /* Initialize map with default values. */
    for(i = 0; i < MAP_SIZE; i++) 
    {
        strcpy(map[i].key, "");
        map[i].value = 0;
    }

    /* Open an existing file for reading */
    file = fopen("#12/test.txt", "r");
    
    /* Quit if the file does not exist */
    if (NULL != file) 
    {   
        int k = 0;
        char ch = {0};
        char word[256] = {0};

        while (EOF != ch)
        {   
            ch = fgetc(file);
            
            if ((' ' != ch) && ('.' != ch) && (',' != ch) && ('?' != ch) && ('!' != ch) && (':' != ch) && (';' != ch)) 
            {
                word[k++] = ch; 
            }
            else 
            {
                word[k] = '\0';
                add_key(map, word);
                strcpy(word, "");
                k = 0;
            }
        }

        sort_items_by_apparitions(map);
        print_items(map);
    }
    else 
    {
        printf("The file pointer is NULL.");
    }
    
    return 0;
}
