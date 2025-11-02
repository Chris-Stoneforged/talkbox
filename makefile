CC=gcc
CFLAGS=-MMD -Wall -Wextra -pedantic -std=c11

SRC_DIR=./src
SRC=$(shell find $(SRC_DIR) -name '*.c')
OBJ=$(SRC:%.c=%.o)
DEP=$(SRC:%.c=%.d)

EXE=talkbox

debug: CFLAGS+=-g
debug: $(EXE)

$(EXE): $(OBJ)
	$(CC) -o $@ $^

-include $(DEP)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<
