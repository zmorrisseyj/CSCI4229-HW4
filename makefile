CC = gcc
CFLAGS = -g -Wall
TARGET = hw4

all: $(TARGET)

$(TARGET): $(TARGET).c
	$(CC)	$(CFLAGS)	-o	hw4	$(TARGET).c	-lglut	-lGLU -lGL -lm


clean:
	$(RM) LorenzAttractor
