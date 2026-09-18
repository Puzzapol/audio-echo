#ifndef AUDIO_BUFFER_H
#define AUDIO_BUFFER_H
#include <stddef.h>

typedef struct {
    float *data;
    size_t capacity;
    size_t writeIndex;
} DelayBuffer;

int DelayBuffer_Init(DelayBuffer *buffer, size_t capacity);
void DelayBuffer_Free(DelayBuffer *buffer);

#endif