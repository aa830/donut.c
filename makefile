# Makefile
CC = gcc
CFLAGS = -lm
TARGET = donut

all: $(TARGET)

$(TARGET): donut.c
	$(CC) donut.c -o $(TARGET) $(CFLAGS)

clean:
	rm -f $(TARGET)
