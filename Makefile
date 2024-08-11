CC = gcc
CFLAGS = -I./include
LDFLAGS = -L./lib -lSDL2 -lSDL2main

SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)
TARGET = intel8080.exe

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@