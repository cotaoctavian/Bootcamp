/**************************************************************
 *                      INCLUDES                              *
 **************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <limits.h>

/**************************************************************
 *                      DEFINES                               *
 **************************************************************/

#define BUFFER_SIZE 256

typedef struct _COMPONENT_DATA
{
	int attributes;
	char *name;
	size_t name_size;
} COMPONENT_DATA, *PCOMPONENT_DATA;

/**************************************************************
 *                  FUNCTIONS DECLARATION	 				  *
 **************************************************************/

static int serializeData(COMPONENT_DATA *component_info, void *buffer, size_t buffer_size, int *result);
static int deserialized_data(void *buffer, size_t buffer_size, COMPONENT_DATA *component_info, int *result);

/**************************************************************
 *                  FUNCTIONS DEFINITION	 				  *
 **************************************************************/

/** 
* @brief 	 	 This function serializes the struct into the buffer.
* @param[in]     component_info - is the parameter that is going to be serialized
* @param[in/out] buffer  	    - is the parameter that stores the serialized data
* @param[in]	 buffer_size 	- is the size of the buffer
* @param[in/out] result  	    - saves 0 if it's a success, non-zero if it's an error
* @return 		 The function returns the size of the serialized data
*/
static int serializeData(COMPONENT_DATA *component_info, void *buffer, size_t buffer_size, int *result) 
{
	int buffer_next = 0;
	
	if ((NULL == component_info) || (NULL == buffer) || (0 == buffer_size) || (NULL == result))
	{
		printf("One of the parameters is null.");
		*result = -2;
	} 
	else
	{
		memcpy(buffer + buffer_next, &component_info->attributes, sizeof(int));
		buffer_next += sizeof(int);

		memcpy(buffer + buffer_next, &component_info->name_size, sizeof(component_info->name_size));
		buffer_next += sizeof(component_info->name_size);

		memcpy(buffer + buffer_next, &component_info->name, component_info->name_size);
		buffer_next += component_info->name_size;

		if (buffer_next <= buffer_size) 
		{ 
			*result = 0;
		}
		else 
		{ 
			*result = -1;
		}
	}

	return buffer_next;
}

/**
* @brief 		 Deserialize the data from buffer and save it into the struct
* @param[in] 	 buffer 		- stores the serialized buffer
* @param[in/out] buffer_size 	- stores the size of the serialized buffer
* @param[in/out] component_info - is the struct that is going to be filled with the deserialized data
* @param[in/out] result 		- saves 0 if it's a success, non-zero if it's an error	
* @return 		 returns the size of the deserialized data
*/
static int deserialized_data(void *buffer, size_t buffer_size, COMPONENT_DATA *component_info, int *result)
{	
	int buffer_next = 0;
	
	if ((NULL == component_info) || (NULL == buffer) || (0 == buffer_size) || (NULL == result))
	{
		printf("One of the parameters is null.");
		*result = -2;
	}
	else 
	{
		memcpy(&component_info->attributes, buffer, sizeof(int));
		buffer_next += sizeof(int);
		
		memcpy(&component_info->name_size, buffer + buffer_next, sizeof(component_info->name_size));
		buffer_next += sizeof(component_info->name_size);

		component_info->name = (char *) malloc (component_info->name_size);

		if (NULL != component_info->name) 
		{
			memcpy(&component_info->name, buffer + buffer_next, component_info->name_size);
			buffer_next += component_info->name_size;

			if (buffer_next <= buffer_size) 
			{ 
				*result = 0;
			}
			else 
			{ 
				*result = -1;
			}
		}
		else 
		{
			*result = -1;
		}
	}

	return buffer_next;
} 

int main() 
{	
	int result = INT32_MIN;
	void *buffer = (void *) malloc (BUFFER_SIZE);

	if (NULL != buffer) 
	{
		COMPONENT_DATA data = {0, "", 0};

		data.attributes = 3;
		data.name = strdup("testare1321321321");
		data.name_size = strlen(data.name);

		/* Serialize data */
		int buffer_size = serializeData(&data, buffer, BUFFER_SIZE, &result);

		/* Check if everything went fine */
		if (0 == result)
		{
		/* Allocate memory to struct variable */
			COMPONENT_DATA copy = {0, "", 0};

			/* Deserialize data */
			int res = deserialized_data(buffer, buffer_size, &copy, &result);

			/* Check if everything went fine */
			if (0 == result) 
			{
				printf("The deserialized data is:\n");

				printf("Attributes: %d\n", copy.attributes);
				printf("Name: %s\n", copy.name);
				printf("Name size: %lu\n", copy.name_size);

				printf("Size in bytes of the deserialized data: %d\n", res);
			} 
			else 
			{
				printf("Something went wrong while deserializing the data...\n");
			}
		}
		else 
		{
			printf("Something went wrong while serializing the data...\n");
		}

		free(buffer);
		buffer = NULL;
	}

    return 0;
}
