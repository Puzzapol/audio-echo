#include <stdio.h>
#include <stdlib.h>
#include <portaudio.h>
#include "audio_buffer.h"
#define SAMPLE_RATE         (44100)
#define PA_SAMPLE_TYPE      paFloat32
#define FRAMES_PER_BUFFER   (64)

static int callBack( const void *inputBuffer, void *outputBuffer,
                     unsigned long framesPerBuffer,
                     const PaStreamCallbackTimeInfo* timeInfo,
                     PaStreamCallbackFlags statusFlags,
                     void *userData ){
    (void) timeInfo; 
    (void) statusFlags;
    const float *in = (const float*)inputBuffer;
    float *out = (float*)outputBuffer;
    unsigned long i;
    DelayBuffer *delayBuffer = (DelayBuffer*)userData;

    if( inputBuffer == NULL )
    {
        for( i = 0; i < framesPerBuffer; i++ )
        {
            float delayed_left = delayBuffer->data[delayBuffer->writeIndex];
            delayBuffer->data[delayBuffer->writeIndex] = 0.0f;
            delayBuffer->writeIndex = (delayBuffer->writeIndex + 1) % delayBuffer->capacity;
            *out++ = delayed_left * 0.5f;

            float delayed_right = delayBuffer->data[delayBuffer->writeIndex];
            delayBuffer->data[delayBuffer->writeIndex] = 0.0f;
            delayBuffer->writeIndex = (delayBuffer->writeIndex + 1) % delayBuffer->capacity;
            *out++ = delayed_right * 0.5f;
        }
    }
    else
    {
        for( i = 0; i < framesPerBuffer; i++ )
        {
            float in_left = *in++;
            float delayed_left = delayBuffer->data[delayBuffer->writeIndex];
            delayBuffer->data[delayBuffer->writeIndex] = in_left;
            delayBuffer->writeIndex = (delayBuffer->writeIndex + 1) % delayBuffer->capacity;
            *out++ = in_left + (delayed_left * 0.5f);

            float in_right = *in++;
            float delayed_right = delayBuffer->data[delayBuffer->writeIndex];
            delayBuffer->data[delayBuffer->writeIndex] = in_right;
            delayBuffer->writeIndex = (delayBuffer->writeIndex + 1) % delayBuffer->capacity;
            *out++ = in_right + (delayed_right * 0.5f);
        }
    }
    
    return paContinue;
}

int main(){
    PaStreamParameters inputParameters, outputParameters;
    PaStream *stream;
    PaError err;
   
    err = Pa_Initialize();
    if( err != paNoError ) goto error;
    inputParameters.device = Pa_GetDefaultInputDevice();
    if (inputParameters.device == paNoDevice) {
        fprintf(stderr, "Error: No default input device.\n");
        goto error;
    }
    inputParameters.channelCount = 2;
    inputParameters.sampleFormat = PA_SAMPLE_TYPE;
    inputParameters.suggestedLatency = Pa_GetDeviceInfo( inputParameters.device )->defaultLowInputLatency;
    inputParameters.hostApiSpecificStreamInfo = NULL;

    outputParameters.device = Pa_GetDefaultOutputDevice();
    if (outputParameters.device == paNoDevice) {
        fprintf(stderr,"Error: No default output device.\n");
        goto error;
    }
    outputParameters.channelCount = 2;
    outputParameters.sampleFormat = PA_SAMPLE_TYPE;
    outputParameters.suggestedLatency = Pa_GetDeviceInfo( outputParameters.device )->defaultLowOutputLatency;
    outputParameters.hostApiSpecificStreamInfo = NULL;
    
    DelayBuffer delayBuffer;
    delayBuffer.data = NULL;
    size_t capacity = (size_t)(SAMPLE_RATE * 0.5 * 2);
    if (DelayBuffer_Init(&delayBuffer, capacity) != 0) {
        fprintf(stderr, "Error: Failed to allocate memory for DelayBuffer.\n");
        goto error;
    }
    
    err = Pa_OpenStream(
              &stream,
              &inputParameters,
              &outputParameters,
              SAMPLE_RATE,
              FRAMES_PER_BUFFER,
              0,
              callBack,
              &delayBuffer );
    if (err != paNoError) goto error;

    err = Pa_StartStream(stream);
    if (err != paNoError) goto error;

    printf("Hit ENTER to stop program.\n");
    getchar();

    err = Pa_StopStream(stream);
    if (err != paNoError) goto error;

    err = Pa_CloseStream(stream);
    if (err != paNoError) goto error;

    Pa_Terminate();
    printf("Execution completed.\n");
    DelayBuffer_Free(&delayBuffer);
    return 0;

    error:
    DelayBuffer_Free(&delayBuffer);
    Pa_Terminate();
    fprintf( stderr, "An error occured while using the portaudio stream\n" );
    fprintf( stderr, "Error number: %d\n", err );
    fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( err ) );
    return -1;
}