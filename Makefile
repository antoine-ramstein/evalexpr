CC = gcc
CFLAGS  = -std=c99 -pedantic -Wall -Wextra -Wvla
SRC = src/main.c src/rpn.c src/stack.c src/stdn.c src/fifo.c
OBJ = $(SRC:.c=.o)
TARGET = evalexpr

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

check:
	./tests/check.sh

clean:
	$(RM) $(OBJ) $(TARGET)
