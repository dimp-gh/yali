CC = gcc
CFLAGS  = -g -Wall
SOURCE_DIR = ./src
TEST_DIR = ./tests
REPL_NAME = repl
TEST_NAME = test
REPL_SOURCE = \
	$(SOURCE_DIR)/lexer.c \
	$(SOURCE_DIR)/parser.c \
	$(SOURCE_DIR)/common.c \
	$(SOURCE_DIR)/hashtable.c \
	$(SOURCE_DIR)/evaluator.c \
	$(SOURCE_DIR)/corelib.c \
	$(SOURCE_DIR)/repl.c

TEST_SOURCE = \
	$(SOURCE_DIR)/lexer.c \
	$(SOURCE_DIR)/parser.c \
	$(SOURCE_DIR)/common.c \
	$(SOURCE_DIR)/hashtable.c \
	$(SOURCE_DIR)/evaluator.c \
	$(SOURCE_DIR)/corelib.c \
	$(TEST_DIR)/test.c


default: repl

repl:	clean	
	$(CC) $(CFLAGS) -o $(REPL_NAME) $(REPL_SOURCE)

test:	clean
	$(CC) $(CFLAGS) -o $(TEST_NAME) $(TEST_SOURCE) 

tags:		
	ctags -e -R .

clean:		
	rm -f $(TEST_NAME) $(REPL_NAME) *.o *~
