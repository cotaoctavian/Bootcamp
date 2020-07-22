#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

/* 
    COMMANDS:
        -> open file - introducerea de la tst file name; -> done
        -> save file - salveaza continutul curent al fisierului in acelasi fisier pe disc -> done
        -> print file content - afiseaza textul -> done
        -> write text at position - introduci textul ce trebuie inserat si pozitia la care se insereaza -> done
        -> delete text from position - pozitia de la care se sterge si nr de caractere care trebuie sterse -> done
        -> undo - sa anuleze efectul ultimei operatie de insertie sau stergere text -> done
        -> redo - sa anuleze efectul ultimului undo -> done
*/

/* 
    How to do it?
        -> implement a stack maybe (for undo / redo);

    Undo delete -> Get the text back.
    Undo insert -> Get the text back.

    Redo delete -> Delete from position x to y.
    Redo insert -> Insert from position x to y.
    
*/

/* Stack's struct */
typedef struct _Stack 
{
    int top; /* top of the stack */
    unsigned size; /* size of the stack */
    char **current_data; /* text written/deleted by the action */
    char **previous_data; /* text before write / delete */
}Stack;


/* 
    * @brief - Initialize a new stack. 
    * @param[in] - unsigned capacity - the parameter that stores the capacity of the queue
    * @return - returns the initialized stack.
*/
Stack *init(unsigned size) 
{
    Stack *stack = (Stack *) malloc (sizeof(Stack));
    stack->top = -1;
    stack->size = size;
    stack->current_data = (char **) malloc (sizeof(char *) * size);
    stack->previous_data = (char **) malloc (sizeof(char *) * size);

    return stack;
}

/* 
    * @brief - Check if the stack is empty.
    * @param[in] - Stack *stack is the stack
    * @return - returns false/true
*/
bool is_empty(Stack *stack)
{
    return stack->top == -1;
}

/* 
    * @brief - Check if the stack is full.
    * @param[in] - Stack *stack is the stack
    * @return - returns false/true
*/
bool is_full(Stack *stack) 
{
    return stack->top == (stack->size - 1);
}

/* 
    * @brief - Add item to the stack
    * @param[in] - char *current_data and char *previous_data are the items that are going to be added into the stack
    * @param[in/out] - Stack *stack is the modified stack
*/
void push(Stack *stack, char *current_data, char *previous_data) 
{ 
    if (is_full(stack)) return;

    ++stack->top;
    stack->current_data[stack->top] = (char *) malloc (sizeof(current_data)); 
    stack->previous_data[stack->top] = (char *) malloc (sizeof(previous_data)); 

    strcpy(stack->current_data[stack->top], current_data); 
    strcpy(stack->previous_data[stack->top], previous_data);
} 

/* 
    * @brief - Remove item from the stack
    * @param[in/out] - Stack *stack is going to be modified because of the removed item
    *                - char *current_data stores the value of the stack->current_data
    *                - char *previous_data stores the value of the stack->previous_data
    * @return - returns 1
*/
int pop(Stack *stack, char *current_data, char *previous_data) 
{ 
    if (is_empty(stack)) return INT32_MIN; 
    
    strcpy(current_data, stack->current_data[stack->top]);
    strcpy(previous_data, stack->previous_data[stack->top]);

    stack->top--;

    return 1;
} 

/* 
    * @brief - Check if a string is a number.
    * @return - returns false/true
*/
bool is_number(char *str) 
{
    int i;
    for (i = 0; str[i]; i++) 
        if (!isdigit(str[i])) return false;
    
    return true;
}

/* 
    * @brief - This function gets the text from the file
    * @param[in] - FILE *f is the pointer of the file
    * @return - returns the text
*/
char* get_text_content(FILE *f) 
{
     /* Get the number of bytes */
    fseek(f, 0L, SEEK_END);
    size_t numbytes = ftell(f);
    
    /* Reset the file position indicator to the beginning of the file */
    fseek(f, 0L, SEEK_SET);	
    
    /* Allocate sufficient memory for the buffer to hold the text */
    char *text = (char*) calloc (numbytes, sizeof(char));	

    /* Memory error */
    if (text == NULL)
        return "NO";
    
    /* Copy all the text into the buffer */
    fread(text, sizeof(char), numbytes, f);

    return text;
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
        if (strcmp(command, "quit") == 0) 
        {
            printf("Bye, bye! \n");
            printf("\n---------------------------------------\n\n");
            break;
        }
        /* ----------------------------------- OPEN COMMAND -------------------------------------- */
        else if (strcmp(command, "open") == 0) 
        {
            printf("Enter the filename:\n");
            getline(&response, &responseSize, stdin);

            response[strcspn(response, "\n")] = 0;

            strcpy(current_file, response);

            file = fopen(response, "r+");

            /* Quit if the file does not exist */
            if (file == NULL)
            {
                printf("%s\n", "The file does not exist. Try another command.");
                printf("\n---------------------------------------\n\n");
            } 
            else 
            {
                printf("%s\n", "File opened. Ready for edit.");
                printf("\n---------------------------------------\n\n");
                access = true;
            }
        }
        /* ----------------------------------- SAVE COMMAND -------------------------------------- */
        else if (strcmp(command, "save") == 0) 
        {
            fclose(file);
            printf("%s\n", "File saved. Choose another file to edit.");
            printf("\n---------------------------------------\n\n");

            access = false; 
            strcpy(current_file, "");
        }
        /* ----------------------------------- PRINT COMMAND -------------------------------------- */
        else if (strcmp(command, "print") == 0) 
        {
            if (access) 
            {
                printf("%s\n", "The content of the file is:");

                char *res = get_text_content(file);
                if (strcmp(res, "NO") == 0) 
                {
                    printf("Something went wrong..\n");
                } 
                else 
                {
                    printf("%s\n", res);
                }

                printf("\n---------------------------------------\n\n");
            } 
            else 
            {
                printf("No file opened. Open a file to have access to this command.\n");
                printf("\n---------------------------------------\n\n");
            }
        } 
        /* ----------------------------------- WRITE COMMAND -------------------------------------- */
        else if (strcmp(command, "write") == 0) 
        {
           if (access) 
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

                    if (is_number(position)) break;
                    else 
                    {
                        printf("Invalid response. Only numbers accepted. Try again.\n");
                        printf("Write the position that you want to start with: \n");
                    }
                }

                int pos = atoi(position);

                /* Allocate sufficient memory for the buffer to hold the text */
                char *prev_text = get_text_content(file);

                if (strcmp(prev_text, "NO") == 0) 
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

                    if (strcmp(curr_text, "NO") == 0) 
                    {
                        printf("Something went wrong..\n");
                    } 
                    else 
                    {
                        /* Add action to stack */
                        if (!is_full(stack_undo)) 
                            push(stack_undo, curr_text, prev_text);
                        
                        free(curr_text);
                        free(prev_text);
                        free(position);

                        printf("The text has been written.\n");
                        printf("\n---------------------------------------\n\n");
                    }
                }

            } 
            else 
            {
                printf("No file opened. Open a file to have access to this command.\n");
                printf("\n---------------------------------------\n\n");
            }
        } 
        /* ----------------------------------- DELETE COMMAND -------------------------------------- */
        else if (strcmp(command, "delete") == 0) 
        { 
            if (access) 
            {
                printf("Write the start position from where you want to delete:\n");

                size_t position_start_size, position_finish_size;

                /* Allocate memory */
                char *position_start = (char *) malloc (POSITION_SIZE);
                char *position_finish = (char *) malloc (POSITION_SIZE);

                /* Read data until you get a number */
                while (true) 
                {
                    getline(&position_start, &position_start_size, stdin);

                    position_start[strcspn(position_start, "\n")] = 0;

                    if (is_number(position_start)) break;
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

                    if (is_number(position_finish)) break;
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

                if (strcmp(prev_text, "NO") == 0) 
                {
                    printf("Something went wrong..\n");
                } 
                else 
                {
                    fclose(file);

                    /* Open it for writing */
                    file = fopen(current_file, "w");

                    // Return if could not open file 
                    if (file == NULL)
                    {
                        printf("%s\n", "The file does not exist. Try another command.");
                        printf("\n---------------------------------------\n\n");
                    } 
                    else 
                    {
                        int i, k = 0;
                        char curr_text[256] = "";
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
                        if (!is_full(stack_undo)) 
                            push(stack_undo, curr_text, prev_text);

                        fclose(file);

                        file = fopen(current_file, "r+");

                        printf("The text has been deleted.\n");
                        printf("\n---------------------------------------\n\n");

                        free(position_start);
                        free(position_finish);
                    }
                }
            }
            else 
            {
                printf("No file opened. Open a file to have access to this command.\n");
                printf("\n---------------------------------------\n\n");
            }
        } 
        /* ----------------------------------- UNDO COMMAND -------------------------------------- */
        else if (strcmp(command, "undo") == 0)
        {
            if (access)
            {
                if (!is_empty(stack_undo)) 
                { 
                    char *curr_data = (char *) malloc (MAX_TEXT_SIZE);
                    char *prev_data = (char *) malloc (MAX_TEXT_SIZE);

                    /* Remove action from undo's stack and add it to redo's stack */
                    if (pop(stack_undo, curr_data, prev_data)) 
                    {
                        push(stack_redo, prev_data, curr_data);
                    }

                    /* Undo the text from the file too */
                    /* Open it for writing */
                    file = fopen(current_file, "w");

                    // Return if could not open file 
                    if (file == NULL) 
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

                        free(prev_data);
                        free(curr_data);

                        printf("The undo command has been succesfully executed.\n");
                        printf("\n---------------------------------------\n\n");
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
        else if (strcmp(command, "redo") == 0) 
        {
            if (access) 
            {
                if (!is_empty(stack_redo)) 
                { 
                    char *curr_data = (char *) malloc (MAX_TEXT_SIZE);
                    char *prev_data = (char *) malloc (MAX_TEXT_SIZE);

                    /* Remove action from undo's stack and add it to redo's stack */
                    if (pop(stack_redo, curr_data, prev_data)) 
                    {
                        push(stack_undo, prev_data, curr_data);
                    }

                    /* Undo the text from the file too */
                    /* Open it for writing */
                    file = fopen(current_file, "w");

                    // Return if could not open file 
                    if (file == NULL) 
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

                        free(prev_data);
                        free(curr_data);

                        printf("The redo command has been succesfully executed.\n");
                        printf("\n---------------------------------------\n\n");
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
    }

    return 0;
}
