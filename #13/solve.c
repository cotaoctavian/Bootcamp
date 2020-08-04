/**************************************************************
 *                      INCLUDES                              *
 **************************************************************/

#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

/**************************************************************
 *                      DEFINES                               *
 **************************************************************/

typedef struct _Stack 
{
    int top; /* top of the stack */
    unsigned size; /* size of the stack */
    char **current_data; /* text written/deleted by the action */
    char **previous_data; /* text before write / delete */
}Stack;

/**************************************************************
 *                FUNCTIONS DECLARATION                       *
 **************************************************************/

static Stack *init(unsigned int size);
static bool is_empty(Stack *stack);
static bool is_full(Stack *stack);
static bool is_number(char *str);
static void push(Stack *stack, char *current_data, char *previous_data);
static int pop(Stack *stack, char *current_data, char *previous_data);
static char *get_text_content(FILE *file);
static void deinitialize(Stack *stack);

/**************************************************************
 *                FUNCTIONS DEFINITION                        *
 **************************************************************/

/**
* @brief     Initialize a new stack. 
* @param[in] capacity - the parameter that stores the capacity of the queue
* @return    returns the initialized stack.
*/
static Stack *init(unsigned int size) 
{
    Stack *stack = (Stack *) malloc (sizeof(Stack));
    stack->top = -1;
    stack->size = size;
    stack->current_data = (char **) malloc (sizeof(char *) * size);
    stack->previous_data = (char **) malloc (sizeof(char *) * size);

    return stack;
}

/** 
* @brief     Check if the stack is empty.
* @param[in] stack - is the stack
* @return    returns false/true
*/
static bool is_empty(Stack *stack)
{
    bool result = false;

    if (NULL != stack) 
    {
        result = stack->top == -1 ? true : false;
    }
    else 
    {
        printf("The stack is NULL.");
        result = false;
    }

    return result;
}

/** 
* @brief     Check if the stack is full.
* @param[in] stack - is the stack
* @return    returns false/true
*/
static bool is_full(Stack *stack) 
{
    bool result = false;

    if (NULL != stack) 
    {
        result = stack->top == (stack->size - 1) ? true : false;
    }
    else 
    {   
        printf("The stack is NULL");
        result = true;
    }

    return result;
}

/** 
* @brief         Add item to the stack
* @param[in]     current_data  - item that is going to be added to the stack
* @param[in]     previous_data - item that is going to be added to the stack
* @param[in/out] stack         - is the modified stack
*/
static void push(Stack *stack, char *current_data, char *previous_data) 
{ 
    if (NULL != stack) 
    {
        if (false == is_full(stack))
        {
            ++stack->top;
            stack->current_data[stack->top] = (char *) malloc (sizeof(current_data)); 
            stack->previous_data[stack->top] = (char *) malloc (sizeof(previous_data)); 

            strcpy(stack->current_data[stack->top], current_data); 
            strcpy(stack->previous_data[stack->top], previous_data);
        }
        else
        {
            printf("The stack is full.");
        }
    }
    else 
    {
        printf("The stack is NULL.");
    }
} 

/** 
* @brief         Remove item from the stack
* @param[in/out] stack         - is going to be modified because of the removed item
* @param[in/out] current_data  - stores the value of the stack->current_data
* @param[in/out] previous_data - stores the value of the stack->previous_data
* @return        returns 1
*/
static int pop(Stack *stack, char *current_data, char *previous_data) 
{   
    int result = 0;

    if (NULL != stack) 
    {
        if (true == is_empty(stack)) 
        {
            result = INT32_MIN;
        }
        else 
        {
            strcpy(current_data, stack->current_data[stack->top]);
            strcpy(previous_data, stack->previous_data[stack->top]);

            stack->top--;

            result = 1;
        }
    }   
    else
    {
        printf("The stack is NULL.");
        result = INT32_MIN;
    }
    
    return result;
} 

/**
 * @brief     Deinitialize the stack.
 * @param[in] stack - is the stack that is going to be deinitialized
 */
static void deinitialize(Stack *stack) 
{
    if (NULL != stack)
    {
        if (NULL != stack->current_data)
        {
            free(stack->current_data);
            stack->current_data = NULL;
        }

        if (NULL != stack->previous_data)
        {
            free(stack->previous_data);
            stack->previous_data = NULL;
        }

        free(stack);
        stack = NULL;
    }
    else
    {
        printf("The stack is NULL.");
    }
}

/** 
* @brief     Check if a string is a number.
* @param[in] str - is the string that is going to be check if it's a number
* @return    returns false/true
*/
static bool is_number(char *str) 
{   
    bool result = true;

    if (NULL != str) 
    {
        int i = 0;

        for (i = 0; str[i]; i++) 
        {
            if (false == isdigit(str[i])) 
            {
                result = false;
            }
        }
    }
    else 
    {
        result = false;
    }

    return result;
}

/** 
* @brief     This function gets the text from the file
* @param[in] f - is the pointer of the file
* @return    returns the text
*/
static char *get_text_content(FILE *file) 
{   
    char *result = (char *) malloc (256);

    if (NULL != file) 
    {
        /* Get the number of bytes */
        fseek(file, 0L, SEEK_END);
        size_t numbytes = ftell(file);
        
        /* Reset the file position indicator to the beginning of the file */
        fseek(file, 0L, SEEK_SET);	
        
        /* Allocate sufficient memory for the buffer to hold the text */
        char *text = (char *) calloc (numbytes, sizeof(char));	

        /* Memory error */
        if (text == NULL)
        {
            strcpy(text, "NO");
        }
        
        /* Copy all the text into the buffer */
        fread(text, sizeof(char), numbytes, file);

        strcpy(result, text);
    }
    else 
    {
        strcpy(result, "NO");
    }

    return result;
}

int main() {
    
    size_t commandSize, responseSize;
    enum { POSITION_SIZE = 8, COMMAND_SIZE = 16, RESPONSE_SIZE = 32, MAX_TEXT_SIZE = 128, STACK_SIZE = 1024 };

    Stack *stack_undo = init(STACK_SIZE);
    Stack *stack_redo = init(STACK_SIZE);

    char *command = (char *) malloc (COMMAND_SIZE);
    char *response = (char *) malloc (RESPONSE_SIZE); 
    char *current_file = (char *) malloc (RESPONSE_SIZE);
    bool access = false;

    if ((NULL == stack_undo) || (NULL == stack_redo) || (NULL == command) || (NULL == response) || (NULL == current_file)) 
    {
        printf("Something went wrong while allocating memory.");
    }
    else 
    {
        printf("Hi, I'm your editor! =)\n");
        printf("Waiting for your commands...\n");
        printf("\n---------------------------------------\n\n");

        FILE *file;

        while (true) 
        {
            printf("Command me: ");

            getline(&command, &commandSize, stdin);

            command[strcspn(command, "\n")] = 0;

            /* ----------------------------------- QUIT COMMAND -------------------------------------- */
            if (0 == strcmp(command, "quit")) 
            {
                printf("Bye, bye! \n");
                printf("\n---------------------------------------\n\n");
                break;
            }
            /* ----------------------------------- OPEN COMMAND -------------------------------------- */
            else if (0 == strcmp(command, "open")) 
            {
                printf("Enter the filename:\n");
                getline(&response, &responseSize, stdin);

                response[strcspn(response, "\n")] = 0;

                strcpy(current_file, response);

                file = fopen(response, "r+");

                /* Quit if the file does not exist */
                if (NULL == file)
                {
                    printf("The file does not exist. Try another command.");
                    printf("\n---------------------------------------\n\n");
                } 
                else 
                {
                    printf("File opened. Ready for edit.");
                    printf("\n---------------------------------------\n\n");
                    access = true;
                }
            }
            /* ----------------------------------- SAVE COMMAND -------------------------------------- */
            else if (0 == strcmp(command, "save")) 
            {
                fclose(file);
                printf("File saved. Choose another file to edit.");
                printf("\n---------------------------------------\n\n");

                access = false; 
                strcpy(current_file, "");
            }
            /* ----------------------------------- PRINT COMMAND -------------------------------------- */
            else if (0 == strcmp(command, "print")) 
            {
                if (true == access) 
                {
                    printf("%s\n", "The content of the file is:");

                    char *res = get_text_content(file);

                    if (0 == strcmp(res, "NO")) 
                    {
                        printf("Something went wrong..\n");
                    } 
                    else 
                    {
                        printf("%s\n", res);
                    }

                    printf("\n---------------------------------------\n\n");

                    if (NULL != res) 
                    {
                        free(res);
                        res = NULL;
                    }
                } 
                else 
                {
                    printf("No file opened. Open a file to have access to this command.\n");
                    printf("\n---------------------------------------\n\n");
                }
            } 
            /* ----------------------------------- WRITE COMMAND -------------------------------------- */
            else if (0 == strcmp(command, "write")) 
            {
                if (true == access) 
                {
                    printf("Write the text you want to insert:\n");

                    getline(&response, &responseSize, stdin);

                    response[strcspn(response, "\n")] = 0;

                    printf("Write the position that you want to start with: \n");

                    char *position = (char *) malloc (POSITION_SIZE);
                    size_t positionSize;

                    while (true) 
                    {
                        getline(&position, &positionSize, stdin);

                        position[strcspn(position, "\n")] = 0;

                        if (true == is_number(position))
                        {
                            break;
                        }
                        else 
                        {
                            printf("Invalid response. Only numbers accepted. Try again.\n");
                            printf("Write the position that you want to start with: \n");
                        }
                    }

                    int pos = atoi(position);

                    /* Allocate sufficient memory for the buffer to hold the text */
                    char *prev_text = get_text_content(file);

                    if (0 == strcmp(prev_text, "NO")) 
                    {
                        printf("Something went wrong..\n");
                    } 
                    else 
                    {
                        // Set the stream pointer "pos" bytes from the start.
                        fseek(file, pos, SEEK_SET); 

                        // Insert response into the file
                        fputs(response, file);

                        /* Allocate sufficient memory for the buffer to hold the text */
                        char *curr_text = get_text_content(file);

                        if (0 == strcmp(curr_text, "NO")) 
                        {
                            printf("Something went wrong..\n");
                        } 
                        else 
                        {
                            /* Add action to stack */
                            if (false == is_full(stack_undo)) 
                            {
                                push(stack_undo, curr_text, prev_text);
                            }
                            
                            if (NULL != curr_text) 
                            {   
                                free(curr_text);
                                curr_text = NULL;
                            }
                            
                            if (NULL != prev_text) 
                            {
                                free(prev_text);
                                prev_text = NULL;
                            }

                            if (NULL != position) 
                            {   
                                free(position);
                                position = NULL;
                            }

                            printf("The text has been written.\n");
                            printf("\n---------------------------------------\n\n");
                        }
                    }

                    if (NULL != prev_text) 
                    {
                        free(prev_text);
                        prev_text = NULL;
                    }
                } 
                else 
                {
                    printf("No file opened. Open a file to have access to this command.\n");
                    printf("\n---------------------------------------\n\n");
                }
            } 
            /* ----------------------------------- DELETE COMMAND -------------------------------------- */
            else if (0 == strcmp(command, "delete")) 
            { 
                if (true == access) 
                {
                    printf("Write the start position from where you want to delete:\n");

                    size_t position_start_size = 0;
                    size_t position_finish_size = 0;

                    /* Allocate memory */
                    char *position_start = (char *) malloc (POSITION_SIZE);
                    char *position_finish = (char *) malloc (POSITION_SIZE);

                    /* Read data until you get a number */
                    while (true) 
                    {
                        getline(&position_start, &position_start_size, stdin);

                        position_start[strcspn(position_start, "\n")] = 0;

                        if (true == is_number(position_start)) 
                        {
                            break;
                        }
                        else 
                        {
                            printf("Invalid response. Only numbers accepted. Try again.\n");
                            printf("Write the position that you want to start with: \n");
                        }
                    }

                    printf("Write the finish position:\n");

                    /* Read data until you get a number */
                    while (true) 
                    {
                        getline(&position_finish, &position_finish_size, stdin);

                        position_finish[strcspn(position_finish, "\n")] = 0;

                        if (true == is_number(position_finish)) 
                        {
                            break;
                        }
                        else 
                        {
                            printf("Invalid response. Only numbers accepted. Try again.\n");
                            printf("Write the position that you want to start with: \n");
                        }
                    }

                    /* Save the numbers */
                    int pos_start = atoi(position_start);
                    int pos_finish = atoi(position_finish);
                    
                    /* Allocate sufficient memory for the buffer to hold the text */
                    char *prev_text = get_text_content(file);

                    if (0 == strcmp(prev_text, "NO")) 
                    {
                        printf("Something went wrong..\n");
                    } 
                    else 
                    {
                        fclose(file);

                        /* Open it for writing */
                        file = fopen(current_file, "w");

                        // Return if could not open file 
                        if (NULL == file)
                        {
                            printf("%s\n", "The file does not exist. Try another command.");
                            printf("\n---------------------------------------\n\n");
                        } 
                        else 
                        {
                            int i = 0;
                            int k = 0;
                            char curr_text[256] = {0};

                            for (i = 0; prev_text[i] != '\0'; i++) 
                            {
                                if (i < pos_start || i > pos_finish) 
                                {
                                    fputc(prev_text[i], file); 
                                    curr_text[k++] = prev_text[i];
                                }
                            }
                            
                            curr_text[k] = '\0';

                            /* Add it to the stack */
                            if (false == is_full(stack_undo)) 
                            {
                                push(stack_undo, curr_text, prev_text);
                            }

                            fclose(file);

                            file = fopen(current_file, "r+");

                            printf("The text has been deleted.\n");
                            printf("\n---------------------------------------\n\n");

                            if (NULL != position_start) 
                            {
                                free(position_start);
                                position_start = NULL;
                            }
                            
                            if (NULL != position_finish) 
                            {   
                                free(position_finish);
                                position_finish = NULL;
                            }

                            strcpy(curr_text, "");
                        }
                    }

                    if (NULL != prev_text) 
                    {
                        free(prev_text);
                        prev_text = NULL;
                    }
                }
                else 
                {
                    printf("No file opened. Open a file to have access to this command.\n");
                    printf("\n---------------------------------------\n\n");
                }
            } 
            /* ----------------------------------- UNDO COMMAND -------------------------------------- */
            else if (0 == strcmp(command, "undo"))
            {
                if (true == access)
                {
                    if (false == is_empty(stack_undo)) 
                    { 
                        char *curr_data = (char *) malloc (MAX_TEXT_SIZE);
                        char *prev_data = (char *) malloc (MAX_TEXT_SIZE);

                        if ((NULL != curr_data) && (NULL != prev_data)) 
                        {
                            /* Remove action from undo's stack and add it to redo's stack */
                            if (0 != pop(stack_undo, curr_data, prev_data)) 
                            {
                                push(stack_redo, prev_data, curr_data);
                            }

                            /* Undo the text from the file too */
                            /* Open it for writing */
                            file = fopen(current_file, "w");

                            // Return if could not open file 
                            if (NULL == file) 
                            {
                                printf("No file opened. Open a file to have access to this command.\n");
                                printf("\n---------------------------------------\n\n");
                            }
                            else 
                            {
                                fseek(file, 0L, SEEK_SET);	
                                fputs(prev_data, file);
                                fclose(file);
                                file = fopen(current_file, "r+");

                                if (NULL != prev_data) 
                                {
                                    free(prev_data);
                                    prev_data = NULL;
                                }
                                
                                if (NULL != curr_data) 
                                {
                                    free(curr_data);
                                    curr_data = NULL;
                                }

                                printf("The undo command has been succesfully executed.\n");
                                printf("\n---------------------------------------\n\n");
                            }
                        }
                    } 
                    else 
                    {
                        printf("Execute some commands. No undos available.\n");
                        printf("\n---------------------------------------\n\n");
                    }
                } 
                else 
                {
                    printf("No file opened. Open a file to have access to this command.\n");
                    printf("\n---------------------------------------\n\n");
                }
            
            } 
            /* ----------------------------------- REDO COMMAND -------------------------------------- */
            else if (0 == strcmp(command, "redo")) 
            {
                if (true == access) 
                {
                    if (false == is_empty(stack_redo)) 
                    { 
                        char *curr_data = (char *) malloc (MAX_TEXT_SIZE);
                        char *prev_data = (char *) malloc (MAX_TEXT_SIZE);

                        if ((NULL != curr_data) && (NULL != prev_data)) 
                        {   
                            /* Remove action from undo's stack and add it to redo's stack */
                            if (0 != pop(stack_redo, curr_data, prev_data)) 
                            {
                                push(stack_undo, prev_data, curr_data);
                            }

                            /* Undo the text from the file too */
                            /* Open it for writing */
                            file = fopen(current_file, "w");

                            // Return if could not open file 
                            if (NULL == file) 
                            {
                                printf("No file opened. Open a file to have access to this command.\n");
                                printf("\n---------------------------------------\n\n");
                            }
                            else 
                            {
                                fseek(file, 0L, SEEK_SET);	
                                fputs(prev_data, file);
                                fclose(file);
                                file = fopen(current_file, "r+");

                                if (NULL != prev_data) 
                                {
                                    free(prev_data);
                                    prev_data = NULL;
                                }
                                
                                if (NULL != curr_data) 
                                {
                                    free(curr_data);
                                    curr_data = NULL;
                                }

                                printf("The redo command has been succesfully executed.\n");
                                printf("\n---------------------------------------\n\n");
                            }
                        }
                    } 
                    else 
                    {
                        printf("Execute some commands. No redos available.\n");
                        printf("\n---------------------------------------\n\n");
                    }
                } 
                else 
                {
                    printf("No file opened. Open a file to have access to this command.\n");
                    printf("\n---------------------------------------\n\n");
                }
            }
            else 
            {
                printf("This command doesn't exist. Try another one.\n");
            }
        }
    }

    deinitialize(stack_undo);
    deinitialize(stack_redo);

    if (NULL != command) 
    {
        free(command);
        command = NULL;
    }

    if (NULL != current_file)
    {
        free(current_file);
        current_file = NULL;
    }

    if (NULL != response)
    {
        free(response);
        response = NULL;
    }

    return 0;
}
