#include "audio_buffer.h"
#include <stdlib.h>

int DelayBuffer_Init(DelayBuffer *buffer, size_t capacity) {
    if (buffer == NULL || capacity == 0) {
        return -1;
    }
    buffer->capacity = capacity;
    buffer->writeIndex = 0;
    buffer->data = (float*)calloc(buffer->capacity, sizeof(float));
    if (buffer->data == NULL) {
        return -1;
    }
    return 0;
}

void DelayBuffer_Free(DelayBuffer *buffer) {
    if (buffer != NULL && buffer->data != NULL) {
        free(buffer->data);
        buffer->data = NULL;
    }
}
