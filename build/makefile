CC = gcc
CFLAGS = $(shell pkg-config --cflags sdl2 SDL2_image SDL2_ttf)
LDFLAGS = $(shell pkg-config --libs sdl2 SDL2_image SDL2_ttf)

SRC_DIR = ../src/

SRC = $(wildcard $(SRC_DIR)/*.c)
EXEC_GAME = engine.out

.PHONY: all engine

all: engine

engine: $(SRC)
	$(CC) $(CFLAGS) $(SRC) $(LDFLAGS) -o $(EXEC_GAME)
	./$(EXEC_GAME)