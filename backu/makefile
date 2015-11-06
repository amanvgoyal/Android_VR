#                                                                               
# makefile for Project 1
# 
#                                                                               
#                                                                               

CC=g++
CPPFLAGS=-std=c++11
DEPS = Database.h Token.h Parser.h 

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CPPFLAGS)

BBS: Database.o Token.o Parser.o Main.o
	g++ -std=c++11 -o BBS Database.o Token.o Parser.o Main.o
#	g++ -o BBS Database.o Token.o Parser.o Main.o

.PHONY: clean

clean:
	rm -f *.o


