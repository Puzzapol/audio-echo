CC = gcc
CFLAGS = -Wall -Wextra -O2 -Isrc
LDFLAGS = -lportaudio -lm

TARGET = echo_effect.exe
SRCS = src/main.c src/audio_buffer.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f src/*.o $(TARGET)

.PHONY: all clean
