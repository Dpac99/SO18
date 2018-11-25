# Makefile, versao 1
# Sistemas Operativos, DEI/IST/ULisboa 2018-19

SOURCES = CircuitRouter-AdvancedShell.c 
SOURCES+= ../lib/commandlinereader.c ../lib/vector.c
OBJS = $(SOURCES:%.c=%.o)
CC   = gcc
CFLAGS =-Wall -g -std=gnu99 -I../
LDFLAGS=-lm
TARGET = CircuitRouter-AdvancedShell 

all: $(TARGET) CircuitRouter-SeqSolver CircuitRouter-Client

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $(TARGET) $(LDFLAGS) 

CircuitRouter-AdvancedShell.o: CircuitRouter-AdvancedShell.c CircuitRouter-AdvancedShell.h ../lib/commandlinereader.h ../lib/vector.h
CircuitRouter-Client.o : CircuitRouter-Client.c ../lib/commandlinereader.h
../lib/commandlinereader.o: ../lib/commandlinereader.c ../lib/commandlinereader.h
../lib/vector.o: ../lib/vector.c ../lib/vector.h ../lib/types.h ../lib/utility.h

$(OBJS):
	$(CC) $(CFLAGS) -c -o $@ $<

CircuitRouter-SeqSolver:
	(cd ../CircuitRouter-SeqSolver/ && make)	

CircuitRouter-Client: CircuitRouter-Client.o 
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

clean:
	@echo Cleaning...
	rm -f $(OBJS) $(TARGET) 
	rm -f $(OBJS) CircuitRouter-Client
	rm -f $(OBJS) CircuitRouter-Client.o
	rm -f *.pipe
	rm -f client*
	(cd ../CircuitRouter-SeqSolver/ && make clean) 
