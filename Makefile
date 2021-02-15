CC=gcc
INCLUDES=-I./include
CFLAGS=-g -Wall -pedantic -fcommon
SRCDIR=./src
OBJS=main.o lexer.o ast.o parser.o
TEST_OBJS=lexer.o ast.o parser.o
NAME=ravenc

%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -o $@ -c $< $(INCLUDES)

all: $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $^ $(INCLUDES)

test: $(TEST_OBJS)
	$(CC) $(CFLAGS) -c ./test/lexer/lexerTest.c $(INCLUDES)
	$(CC) $(CFLAGS) -o testLexer $(TEST_OBJS) lexerTest.o
	./testLexer

.PHONY: clean
clean:
	rm -rf *.dSYM
	rm -f *.o $(NAME) vgcore.* massif.*
	rm -f testLexer