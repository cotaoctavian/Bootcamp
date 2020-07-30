/**************************************************************
 *                      INCLUDES                              *
 **************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/**************************************************************
 *                      DEFINES                               *
 **************************************************************/

#define CAPACITY 15

struct Queue 
{
    int top;
    unsigned int size;
    unsigned int capacity;
    int *array;
};

/**************************************************************
 *                FUNCTIONS DECLARATION                       *
 **************************************************************/

struct Queue *initialize(unsigned int capacity);
bool is_empty(struct Queue *queue);
bool is_full(struct Queue *queue);
void push(struct Queue *queue, uint32_t value);
int pop(struct Queue *queue);
int front(struct Queue *queue);
void print_queue(struct Queue *queue);
void deinitialize(struct Queue *queue);

/**************************************************************
 *                FUNCTIONS DEFINITION                        *
 **************************************************************/

/**
* @brief     Initialize a new queue. 
* @param[in] capacity - the parameter that stores the capacity of the queue
* @return    returns the initialized queue
*/
struct Queue *initialize(unsigned int capacity) 
{
    struct Queue *queue = (struct Queue *) malloc (sizeof(struct Queue));
    
    queue->capacity = capacity;
    queue->size = 0;
    queue->array = (int *) malloc (capacity * sizeof(int));
    queue->top = 0;

    return queue;
}

/** 
* @brief     Check if the queue is empty.
* @param[in] queue - is the queue
* @return    returns false/true
*/
bool is_empty(struct Queue *queue)
{
    int ret_val = 0;

    if (NULL != queue) 
    {
        ret_val = queue->size == 0; 
    } 
    else 
    {
        ret_val = -1;
    }

    return ret_val;
}

/** 
* @brief     Check if the queue is full.
* @param[in] queue - is the queue
* @return    returns false/true
*/
bool is_full(struct Queue *queue) 
{
    int ret_val = 0;

    if (NULL != queue)
    {
        ret_val = queue->size == queue->capacity;
    }
    else 
    {
        ret_val = -1;
    }

    return ret_val;
}

/**
* @brief         Add an element to queue.
* @param[in]     value - is the item that is going to be added into the queue
* @param[in/out] queue - is the modified queue
*/ 
void push(struct Queue *queue, uint32_t value) 
{
    if (NULL != queue)
    {
        /* Check if the size of the queue is lower than the capacity of it. */
        if (0 != is_full(queue)) 
        {
            printf("The queue is full.");
        }
        else 
        {
            queue->array[queue->size] = value;
            queue->size += 1;
        }
    } 
    else 
    {
        printf("The queue is NULL.");
    }
}

/** 
* @brief         Remove top element from queue and return it.
* @param[in/out] queue - is the modified queue
* @return        returns the removed element or a negative value in case of an error
*/
int pop(struct Queue *queue) 
{
    int result = 0;

    if (NULL != queue) 
    {
        if (0 != is_empty(queue)) 
        {
            printf("The queue is empty. Invalid operation.");
            result = INT32_MIN;
        } 
        else
        {
            int element = 0; 
            element = queue->array[queue->top];

            int i = 0;

            for (i = 0; i <= queue->size - 1; i++) 
            {
                queue->array[i] = queue->array[i + 1];
            }

            queue->size -= 1;
            
            result = element;
        }
    }
    else 
    {
        result = INT32_MIN;
    }
    return result;
}

/**
* @brief     Get the top element.
* @param[in] queue - is the queue
* @return    returns the top element or a negative value in case of an error
*/
int front(struct Queue *queue) 
{
    int result = 0;

    if (NULL != queue)
    {
         if (0 != is_empty(queue)) 
        {   
            result = INT32_MIN;
        }
        else 
        {
            result = queue->array[queue->top];
        }
    }
    else
    {
        result = INT32_MIN;
    }
    
    return result;
}

/** 
* @brief     Print the elements of the queue.
* @param[in] queue - is the queue
*/
void print_queue(struct Queue *queue)
{   
    if (NULL != queue)
    {
        int i = 0;
        printf("The queue values are: ");

        for (i = 0; i <= queue->size - 1; i++) 
        {
            printf("%d ", queue->array[i]);
        }

        printf("\n");
    }   
    else 
    {
        printf("The queue is NULL.");
    }
}

/**
* @brief     Deallocate the memory for the struct. 
* @param[in] queue - is the queue.
*/
void deinitialize(struct Queue *queue) 
{
    if (NULL != queue) 
    {   
        free(queue);
        queue = NULL;

        if (NULL != queue->array) 
        {
            free(queue->array);
            queue->array = NULL;
        }
    }
    else 
    {
        printf("The queue is NULL.");
    }
}

int main() 
{
    int top = 0;
    int result = 0;
    struct Queue *queue = initialize(CAPACITY);

    if (NULL != queue) 
    {
        push(queue, 6);
        push(queue, 67);
        push(queue, 21);
        push(queue, 3);

        print_queue(queue);
        top = front(queue);
        printf("The front element is: %d\n", top);

        result = pop(queue);
        printf("The removed item is: %d\n", result);
        print_queue(queue);

        top = front(queue);
        printf("The front element is: %d\n", front(queue));

        deinitialize(queue);
    }
    else 
    {
        printf("The queue is NULL.");
    }

    return 0;
}
