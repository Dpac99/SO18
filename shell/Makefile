#This Makefile compiles the Solver as well.
#Always run the shell from this directory.

CFLAGS=  -Wall -std=gnu99


CircuitRouter-SimpleShell:  ../lib/process.o ../lib/commandlinereader.o CircuitRouter-SimpleShell.o
		make -C ../solver
		gcc $(CFLAGS) ../lib/list.o ../lib/process.o ../lib/commandlinereader.o CircuitRouter-SimpleShell.o -lm -o CircuitRouter-SimpleShell

CircuitRouter-SimpleShell.o: CircuitRouter-SimpleShell.c  ../lib/list.h ../lib/commandlinereader.h ../lib/process.h
		gcc $(CFLAGS) -c CircuitRouter-SimpleShell.c -o CircuitRouter-SimpleShell.o

process.o: ../lib/process.c ../lib/process.h
		gcc $(CFALGS) -c process.c -o process.o

commandlinereader.o: ../lib/commandlinereader.c ../lib/commandlinereader.h
		gcc $(CFLAGS) -c commandlinereader.c -o commandlinereader.o

clean:
		make clean -C ../solver
		rm -f *.o CircuitRouter-SimpleShell
		
