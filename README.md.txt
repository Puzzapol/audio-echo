# Audio Echo Effect

## Description

This project implements a real-time audio echo effect in C.

The program captures audio from the default microphone, applies a delay using a circular buffer, and plays the processed audio through the speakers.

The echo uses a delay of approximately 0.4 seconds and the delayed signal has a volume of 50% compared to the original signal.

## Requirements

The project uses:

* C
* PortAudio
* Make

The audio configuration is:

* Sample rate: 44100 Hz
* Mono audio
* Float audio samples

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

The program uses a circular buffer to implement the audio delay.

For each input sample:

1. The delayed sample is read from the circular buffer.
2. The current microphone sample is written into the buffer.
3. The output is calculated as:

```text
output = current_sample + (delayed_sample * 0.5)
```

4. The buffer position is advanced. When the end of the buffer is reached, it wraps around to the beginning.

The buffer size determines the delay:

```text
delay = buffer_size / sample_rate
```

For example, with a sample rate of 44100 Hz and a delay of approximately 0.4 seconds:

```text
buffer_size = 0.4 * 44100 = 17640 samples
```

## Build

From the project root, run:

```bash
make
```

This compiles the source files and creates the executable.

## Run

After compiling, run the generated executable:

```bash
./audio_echo
```

The program uses the default microphone as input and the default audio output as the speaker.

To hear the effect, speak into the microphone while the program is running.

## Implementation

The project is divided into separate components.

### main.c

Handles:

* PortAudio initialization
* Opening the audio stream
* The audio callback
* Starting and stopping the audio stream

### audio_buffer.c / audio_buffer.h

Implement the circular buffer used to store audio samples and retrieve delayed samples.

The circular buffer avoids continuously allocating and moving memory. The read and write positions wrap around when they reach the end of the buffer.

## Memory Management

The circular buffer is dynamically allocated when it is created and released when it is no longer needed.

The PortAudio stream is also properly stopped and terminated when the program exits.

## Notes

The audio processing is performed inside the PortAudio callback. The callback does not perform operations such as `printf` or dynamic memory allocation, since these operations can interfere with real-time audio processing.
