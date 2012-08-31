CC = gcc
CFLAGS  = -g -Wall

default: lambda


hash:		linklist.o hashtable.o testtable.o
		$(CC) $(CFLAGS) -o hash linklist.o hashtable.o testtable.o

list:		linklist.o testlist.o
		$(CC) $(CFLAGS) -o list linklist.o testlist.o

lambda:		common.o lambdatest.o sexpr.o parser.o hashtable.o linklist.o
		$(CC) $(CFLAGS) -o lambda common.o sexpr.o lambdatest.o parser.o hashtable.o linklist.o

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

clean:
		rm -f testhash testlist lambda *.o *~
