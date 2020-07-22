#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <limits.h>

#define BUFFER_SIZE 25

typedef struct _COMPONENT_DATA
{
	int attributes;
	char *name;
	size_t nameSize;
} COMPONENT_DATA, *PCOMPONENT_DATA;

int serializeData(COMPONENT_DATA* componentInfo, void *buffer, size_t bufferSize, int *result) 
{

	/*
	componentInfo - IN - has all the members initialized and filled with valid data
	buffer - IN/OUT - is allocated by the caller before calling this function and will contain the serialized data if big enough
	bufferSize - IN - size in bytes of the already allocated buffer passed to this function
	result - IN/OUT - 0 success , non-zero error -> NOT ADDED
	return value - OUT - size in bytes of the serialized data that must be filled in buffer
	*/

	int bufferNext = 0;
	memcpy(buffer + bufferNext, &componentInfo->attributes, sizeof(int));
	bufferNext += sizeof(int);

	memcpy(buffer + bufferNext, &componentInfo->name, sizeof(componentInfo->name));
	bufferNext += sizeof(componentInfo->name);

	memcpy(buffer + bufferNext, &componentInfo->nameSize, sizeof(componentInfo->nameSize));
	bufferNext += sizeof(componentInfo->nameSize);

	if (bufferNext < bufferSize) *result = 0;
	else *result = -1;

	return bufferNext;
}

int deserializeData(void *buffer, size_t bufferSize, COMPONENT_DATA *componentInfo, int *result)
{
	/*
	buffer - IN - a buffer that is already allocated and contains the serialized data
	bufferSize - IN - size in bytes of buffer
	componentInfo - IN/OUT - a valid pointer to a componentInfo that will be initialized with the data contained in the buffer
	result - IN/OUT - 0 success , non-zero error -> NOT ADDED
	return value - OUT - size in bytes of the serialized data that must be filled in buffer
	*/

	int bufferNext = 0;
	memcpy(&componentInfo->attributes, buffer, sizeof(int));
	bufferNext += sizeof(int);
	
	memcpy(&componentInfo->name, buffer + bufferNext, sizeof(componentInfo->name));
	bufferNext += sizeof(componentInfo->name);

	memcpy(&componentInfo->nameSize, buffer + bufferNext, sizeof(componentInfo->nameSize));
	bufferNext += sizeof(componentInfo->nameSize);

	if (bufferNext <= bufferSize) *result = 0;
	else *result = -1;

	return bufferSize;
} 

int main() 
{	
	int result = INT32_MIN;

	/* Allocating memory to buffer */
    void *buffer = (void *) malloc (BUFFER_SIZE);

	/* Check if the memory has been allocated */
	if (buffer == NULL) return -1;
	
	/* Allocate memory to struct variable */
	COMPONENT_DATA *data = (COMPONENT_DATA *) malloc (sizeof(COMPONENT_DATA));

	data->attributes = 3;
	data->name = "Test";
	data->nameSize = 4;
	
	/* Serialize data */
	int bufferSize = serializeData(data, buffer, BUFFER_SIZE, &result);

	/* Check if everything went fine */
	if (!result)
	{
		/* Allocate memory to struct variable */
		COMPONENT_DATA *copy = (COMPONENT_DATA *) malloc (sizeof(COMPONENT_DATA));

		copy->attributes = 0;
		copy->name = "";
		copy->nameSize = 0;

		/* Deserialize data */
		int res = deserializeData(buffer, bufferSize, copy, &result);

		/* Check if everything went fine */
		if (!result) 
		{
			printf("The deserialized data is:\n");

			printf("Attributes: %d\n", copy->attributes);
			printf("Name: %s\n", copy->name);
			printf("Name size: %lu\n", copy->nameSize);

			printf("Size in bytes of the serialized data that must be filled in buffer: %d\n", res);
		} 
		else 
		{
			printf("Something went wrong while deserializing the data...");
		}
	}
	else 
	{
		printf("Something went wrong while serializing the data...");
	}

	free(buffer);

    return 0;
}
