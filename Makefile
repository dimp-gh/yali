CC = gcc
CFLAGS  = -g -Wall

default: lambda


hash:		hashtable.o testtable.o
		$(CC) $(CFLAGS) -o hash hashtable.o testtable.o

list:		linklist.o testlist.o
		$(CC) $(CFLAGS) -o list linklist.o testlist.o

lambda:		common.o lambdatest.o sexpr.o parser.o hashtable.o
		$(CC) $(CFLAGS) -o lambda common.o sexpr.o lambdatest.o parser.o hashtable.o

lex:		lexer.o lextest.o
		$(CC) $(CFLAGS) -o lex lexer.o lextest.o 

linklist.o:	linklist.c linklist.h
		$(CC) $(CFLAGS) -o linklist.o -c linklist.c

testlist.o:	testlist.c
		$(CC) $(CFLAGS) -o testlist.o -c testlist.c

hashtable.o:	hashtable.c hashtable.h
		$(CC) $(CFLAGS) -o hashtable.o -c hashtable.c

testtable.o:	testtable.c
		$(CC) $(CFLAGS) -o testtable.o -c testtable.c

lambdatest.o:	lambdatest.c
		$(CC) $(CFLAGS) -o lambdatest.o -c lambdatest.c

sexpr.o:	sexpr.h sexpr.c
		$(CC) $(CFLAGS) -o sexpr.o -c sexpr.c

common.o:	common.c common.h
		$(CC) $(CFLAGS) -o common.o -c common.c

parser.o:	parser.c parser.h
		$(CC) $(CFLAGS) -o parser.o -c parser.c

lexer.o:	lexer.c lexer.h
		$(CC) $(CFLAGS) -o lexer.o -c lexer.c

lextest.o:	lextest.c
		$(CC) $(CFLAGS) -o lextest.o -c lextest.c

clean:
		rm -f hash list lex lambda *.o *~
