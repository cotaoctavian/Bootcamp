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
	size_t name_size;
} COMPONENT_DATA, *PCOMPONENT_DATA;

/* 
	* @brief - This function serializes the struct into the buffer.
	
	* @param[in] - COMPONENT_DATA *component_info is the parameter that is going to be serialized
				 - size_t buffer_size is the size of the buffer

	* @param[in/out] - int *result saves 0 if it's a success, non-zero if it's an error
				 	 - void *buffer is the parameter that stores the serialized data
					  
	* @return - The function returns the size of the serialized data
*/
int serializeData(COMPONENT_DATA *component_info, void *buffer, size_t buffer_size, int *result) 
{
	int buffer_next = 0;
	memcpy(buffer + buffer_next, &component_info->attributes, sizeof(int));
	buffer_next += sizeof(int);

	memcpy(buffer + buffer_next, &component_info->name, sizeof(component_info->name));
	buffer_next += sizeof(component_info->name);

	memcpy(buffer + buffer_next, &component_info->name_size, sizeof(component_info->name_size));
	buffer_next += sizeof(component_info->name_size);

	if (buffer_next < buffer_size) *result = 0;
	else *result = -1;

	return buffer_next;
}

/* 
	* @brief - Deserialize the data from buffer and save it into the struct

	* @param[in] - void *buffer stores the serialized buffer

	* @param[in/out] - COMPONENT_DATA *component_info is the struct that is going to be filled with the deserialized data
					 - int *result saves 0 if it's a success, non-zero if it's an error
					 - size_t buffer_size stores the size of the serialized buffer	

	* return - returns the size of the deserialized data

*/
int deserialized_data(void *buffer, size_t buffer_size, COMPONENT_DATA *component_info, int *result)
{
	int buffer_next = 0;
	memcpy(&component_info->attributes, buffer, sizeof(int));
	buffer_next += sizeof(int);
	
	memcpy(&component_info->name, buffer + buffer_next, sizeof(component_info->name));
	buffer_next += sizeof(component_info->name);

	memcpy(&component_info->name_size, buffer + buffer_next, sizeof(component_info->name_size));
	buffer_next += sizeof(component_info->name_size);

	if (buffer_next <= buffer_size) *result = 0;
	else *result = -1;

	return buffer_size;
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
	data->name_size = 4;
	
	/* Serialize data */
	int buffer_size = serializeData(data, buffer, BUFFER_SIZE, &result);

	/* Check if everything went fine */
	if (!result)
	{
		/* Allocate memory to struct variable */
		COMPONENT_DATA *copy = (COMPONENT_DATA *) malloc (sizeof(COMPONENT_DATA));

		copy->attributes = 0;
		copy->name = "";
		copy->name_size = 0;

		/* Deserialize data */
		int res = deserialized_data(buffer, buffer_size, copy, &result);

		/* Check if everything went fine */
		if (!result) 
		{
			printf("The deserialized data is:\n");

			printf("Attributes: %d\n", copy->attributes);
			printf("Name: %s\n", copy->name);
			printf("Name size: %lu\n", copy->name_size);

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
