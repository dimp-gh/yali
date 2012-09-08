CC = gcc
CFLAGS  = -g -Wall

default: repl

repl:		repl.o common.o lexer.o bnparser.o hashtable.o evaluator.o
		$(CC) $(CFLAGS) -o repl common.o repl.o lexer.o bnparser.o hashtable.o evaluator.o

tests:		test.o common.o lexer.o bnparser.o hashtable.o evaluator.o
		$(CC) $(CFLAGS) -o tests common.o test.o lexer.o bnparser.o hashtable.o evaluator.o

hashtable.o:	hashtable.c hashtable.h
		$(CC) $(CFLAGS) -o hashtable.o -c hashtable.c

common.o:	common.c common.h
		$(CC) $(CFLAGS) -o common.o -c common.c

parser.o:	parser.c parser.h
		$(CC) $(CFLAGS) -o parser.o -c parser.c

lexer.o:	lexer.c lexer.h
		$(CC) $(CFLAGS) -o lexer.o -c lexer.c

lextest.o:	lextest.c
		$(CC) $(CFLAGS) -o lextest.o -c lextest.c

bnparser.o:	bnparser.c bnparser.h
		$(CC) $(CFLAGS) -o bnparser.o -c bnparser.c

evaluator.o:	evaluator.c evaluator.h
		$(CC) $(CFLAGS) -o evaluator.o -c evaluator.c

repl.o:		repl.c
		$(CC) $(CFLAGS) -o repl.o -c repl.c

test.o:		test.c
		$(CC) $(CFLAGS) -o test.o -c test.c

clean:
		rm -f tests repl *.o *~
