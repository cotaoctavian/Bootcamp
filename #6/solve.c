#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define CAPACITY 15

struct Queue 
{
    int top, size;
    int *array;

    unsigned capacity;
};

/* 
    * @brief - Initialize a new queue. 
    * @param[in] - unsigned capacity - the parameter that stores the capacity of the queue
    * @return - returns the initialized queue
*/
struct Queue *initialize(unsigned capacity) 
{
    struct Queue *queue = (struct Queue *)malloc(sizeof(struct Queue));
    
    queue->capacity = capacity;
    queue->size = 0;
    queue->array = (int *)malloc(capacity * sizeof(int));
    queue->top = 0;

    return queue;
}

/* 
    * @brief - Check if the queue is empty.
    * @param[in] - struct Queue *queue is the queue
    * @return - returns false/true
*/
bool is_empty(struct Queue *queue)
{
    return queue->size == 0;
}

/* 
    * @brief - Check if the queue is full.
    * @param[in] - struct Queue *queue is the queue
    * @return - returns false/true
*/
bool is_full(struct Queue *queue) 
{
    return queue->size == queue->capacity;
}

/* 
    * @brief - Add an element to queue.
    * @param[in] - uint32_t value is the item that is going to be added into the queue
    * @param[in/out] - struct Queue *queue is the modified queue
*/ 
void push(struct Queue *queue, uint32_t value) 
{
    /* Check if the size of the queue is lower than the capacity of it. */
    if (is_full(queue)) return;

    queue->array[queue->size] = value;
    queue->size += 1;
}

/* 
    * @brief - Remove top element from queue and return it.
    * @param[in/out] - struct Queue *queue is the modified queue
    * @return - returns the removed element or a negative value in case of an error
*/
int pop(struct Queue *queue) 
{
    if (is_empty(queue)) 
    {
        printf("The queue is empty. Invalid operation.");
        return INT32_MIN;
    } 
    else
    {
        int el = queue->array[queue->top];

        int i;
        for (i = 0; i <= queue->size - 1; i++) 
        {
            queue->array[i] = queue->array[i + 1];
        }

        queue->size -= 1;
        
        return el;
    }
}

/* 
    * @brief - Get the top element.
    * @param[in] - struct Queue *queue is the queue
    * @return - returns the top element or a negative value in case of an error
*/
int front(struct Queue *queue) 
{
    if (is_empty(queue)) return INT32_MIN;
    return queue->array[queue->top];
}

/* 
    * @brief - Print the elements of the queue.
    * @param[in] - struct Queue *queue is the queue
*/
void print_queue(struct Queue *queue)
{
    int i;

    printf("The queue values are: ");
    for (i = 0; i <= queue->size - 1; i++) 
    {
        printf("%d ", queue->array[i]);
    }

    printf("\n");
}


int main() {

    struct Queue *queue = initialize(CAPACITY);

    push(queue, 6);
    push(queue, 67);
    push(queue, 21);
    push(queue, 3);

    print_queue(queue);
    printf("The front element is: %d\n", front(queue));

    printf("The removed item is: %d\n", pop(queue));
    print_queue(queue);

    printf("The front element is: %d\n", front(queue));

    return 0;
}
