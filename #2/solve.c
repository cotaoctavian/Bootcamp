#include <stdio.h>
#include <ctype.h>
#include <stdint.h>
#include <string.h>

uint32_t getValue(char *buffer, int offset, int bufferSize) {

    // Check if the buffer is not empty.
    if(bufferSize == 0) {
        return -1;
    } else if(offset > bufferSize)  { // Check if the offset is not greater than bufferSize.
        return -2;
    } else { // Search for uint32_t value in the buffer.
        int i;
        for(i = offset; buffer[i]; i++) {
            if(isdigit(buffer[i])) {
                return (uint32_t) buffer[i] - 48;
            }
        }

        return -3;
    }

    return 0;
}

int main() {
    char *buffer = "Tes3tare4";

    // Check if the buffer is not NULL;
    if(buffer != NULL) {

        // Get the function's result.
        uint32_t result = getValue(buffer, 5, strlen(buffer) - 1);
        switch(result) {
            case -1:
                printf("%s\n", "The buffer is empty.");
                break;
            case -2:
                printf("%s\n", "The offset is greater than bufferSize.");
                break;
            case -3: 
                printf("%s\n", "The buffer doesn't contain any numbers.");
                break;
            default:
                printf("%d\n", result);
                break;
        }
    } else {
        printf("%s\n", "The buffer is NULL.");
    }
    return 0;
}