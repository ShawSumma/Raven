CC=gcc
INCLUDES=-I./include
CFLAGS=-g -Wall -pedantic -fcommon
SRCDIR=./src
OBJS=main.o lexer.o ast.o parser.o
NAME=ravenc

%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -o $@ -c $< $(INCLUDES)

all: $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $^ $(INCLUDES)

.PHONY: clean
clean:
	rm -rf *.dSYM
	rm -f *.o $(NAME) vgcore.* massif.*