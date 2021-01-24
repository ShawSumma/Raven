CC=gcc
INCLUDES=-I./include
CFLAGS=-g -Wall -pedantic
SRCDIR=./src
OBJS=main.o lexer.o
NAME=ravenc

%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -o $@ -c $< $(INCLUDES)

all: $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $^ $(INCLUDES)

.PHONY: clean
clean:
	rm -rf *.dSYM
	rm *.o
	rm $(NAME)