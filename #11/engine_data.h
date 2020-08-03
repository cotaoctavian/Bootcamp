#ifndef ENGINE_H

/**************************************************************
 *                      DEFINES                               *
 **************************************************************/

#define ENGINE_H

/** 
 * @brief This function allocates memory to ENGINE_DATA struct.
 */
void *initializeEngine(void);

/** 
 * @brief This function starts the consumer thread.
 * @param[in] engine - is the struct that contains the queue & the mutex.
 */
int startEngine(void *engine);

/**
 * @brief     Function that receives an integer, prints it's thread id and index value and calls push into the synchronized queue
 * @param[in] engine - is the struct that contains the queue & the mutex.
 * @param[in] index  - is the number of the thread.
 */
int produce(void *engine, int index);

/**
 * @brief     Function that is called by a consumer thread, extracts the elements from the queue and prints them.
 * @param[in] engine - is the struct that contains the queue & the mutex.
 * @param[in] index  - is the number of the thread.
 */
int onConsume(void *engine, int index);

/** 
 * @brief This function stops the consumer thread.
 * @param[in] engine - is the struct that contains the queue & the mutex.
 */
void stopEngine(void *engine);

/**
 * @brief     Deallocate the memory for ENGINE_DATA struct. 
 * @param[in] engine - is the struct that contains the queue & the mutex.
 */
void destroyEngine(void *engine);

#endif
