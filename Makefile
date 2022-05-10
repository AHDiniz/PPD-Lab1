CC := gcc

EXE := bin/merge

SRC := $(wildcard *.c)

FLAGS := -g
ifeq ($(OS), Windows_NT)
	LIBS := -lgdi32 -lwinmm
else
	LIBS := -lpthread
endif

build: $(wildcard *.c) $(wildcard *.h)
	$(CC) -o $(EXE) $(SRC) $(FLAGS) $(LIBS)
