CC = gcc
CFLAGS = -I./include
LDFLAGS = -L./lib -lSDL2 -lSDL2main

SRC = $(wildcard src/*.c)
OBJ = $(patsubst src/%.c, obj/%.o, $(SRC))
TARGET = intel8080.exe

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LDFLAGS)

obj/%.o: src/%.c
	@if not exist obj mkdir obj
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	-del /Q obj\*.o $(TARGET) 2>nul || rm -rf obj/*.o $(TARGET)
