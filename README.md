# Audio Echo Effect

## Description

This project implements a real-time audio echo effect in C using the PortAudio library.

The program captures audio from the default microphone, applies a delay effect using a circular buffer, and plays the processed audio through the default output device.

The audio stream uses two channels (stereo), a sample rate of 44100 Hz, and a delay of approximately 0.5 seconds.

## Requirements

The project uses:

* C
* PortAudio
* Make

Audio configuration:

* Sample rate: 44100 Hz
* Channels: 2 (stereo)
* Sample format: 32-bit floating point
* Delay: approximately 0.5 seconds

### Install PortAudio

On Linux:

```bash
sudo apt-get install portaudio19-dev
```

On macOS:

```bash
brew install portaudio
```

## Project Structure

```text
project/
├── Makefile
├── src/
│   ├── main.c
│   ├── audio_buffer.c
│   └── audio_buffer.h
└── README.md
```

## How It Works

The program uses a circular buffer to store previous audio samples and create the delay effect.

For every audio frame, the program processes the left and right channels separately.

For each channel:

1. The current input sample is read from the microphone.
2. A delayed sample is read from the circular buffer.
3. The current sample is stored in the same position in the buffer.
4. The output is calculated as:

```text
output = current_sample + (delayed_sample * 0.5)
```

5. The buffer index is advanced using modulo to wrap around when the end of the buffer is reached.

The delayed signal has approximately 50% of the volume of the original signal.

## Circular Buffer

The circular buffer is dynamically allocated using `calloc()`.

The buffer capacity is calculated as:

```text
capacity = sample_rate * delay * channels
```

With the current configuration:

```text
capacity = 44100 * 0.5 * 2
         = 44100 samples
```

Since the audio is stereo, the samples are stored in an interleaved format:

```text
left, right, left, right, ...
```

The `writeIndex` is incremented after each sample and wraps around using the modulo operator:

```c
writeIndex = (writeIndex + 1) % capacity;
```

This allows the program to continuously reuse the same memory without moving the contents of the buffer.

## PortAudio

PortAudio is used to handle the real-time audio input and output.

The program:

1. Initializes PortAudio.
2. Selects the default input and output devices.
3. Opens an audio stream.
4. Processes the audio inside the callback.
5. Starts the stream.
6. Stops and closes the stream when the user presses Enter.
7. Terminates PortAudio.

The callback receives the microphone input and writes the processed samples to the output buffer.

## Build

From the project root, run:

```bash
make
```

The Makefile compiles the source files and creates the executable.

## Run

After compiling, run the generated executable:

```bash
./audio_echo
```

The program uses the default microphone as input and the default audio output as output.

Press **Enter** in the terminal to stop the program.

## Implementation

### `main.c`

This file contains:

* PortAudio initialization and termination
* Input and output device configuration
* Audio stream creation
* The real-time audio callback
* Starting and stopping the audio stream
* Error handling

### `audio_buffer.c`

This file contains the implementation of the circular buffer.

It handles:

* Buffer allocation
* Buffer initialization
* Memory deallocation

### `audio_buffer.h`

This file contains the `DelayBuffer` structure and the declarations of the buffer functions.

## Memory Management

The circular buffer is allocated dynamically during initialization using `calloc()`.

When the program finishes, the allocated memory is released using `free()`.

The PortAudio stream is also properly stopped, closed, and terminated before the program exits.

## Real-Time Audio Considerations

The audio processing is performed inside the PortAudio callback.

The callback does not use functions such as `printf()` or `malloc()`. This is important because blocking operations and dynamic memory allocation can cause problems in real-time audio processing.

The buffer is allocated before the audio stream starts and is then reused during processing.
