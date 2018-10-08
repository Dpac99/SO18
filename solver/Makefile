CFLAGS=  -Wall -std=gnu99

CircuitRouter-SeqSolver: ../components/coordinate.o ../components/grid.o CircuitRouter-SeqSolver.o ../components/maze.o ../components/router.o ../lib/list.o ../lib/list.o ../lib/pair.o ../lib/queue.o ../lib/vector.o
		gcc $(CFLAGS) ../components/coordinate.o ../components/grid.o CircuitRouter-SeqSolver.o ../components/maze.o ../components/router.o ../lib/list.o ../lib/pair.o ../lib/queue.o ../lib/vector.o -lm -o CircuitRouter-SeqSolver

coordinate.o: ../components/coordinate.c ../components/coordinate.h ../lib/pair.h ../lib/types.h
		gcc $(CFLAGS) -c coordinate.c -o coordinate.o

grid.o: ../components/grid.c ../components/grid.h ../components/coordinate.h ../lib/types.h ../lib/vector.h
		gcc $(CFLAGS) -c grid.c -o grid.o
	
CircuitRouter-SeqSolver.o: CircuitRouter-SeqSolver.c ../lib/list.h ../components/maze.h ../components/router.h ../lib/timer.h ../lib/types.h
		gcc $(CFLAGS) -c CircuitRouter-SeqSolver.c -o CircuitRouter-SeqSolver.o

maze.o: ../components/maze.c ../components/maze.h ../components/coordinate.h ../components/grid.h ../lib/list.h ../lib/queue.h ../lib/pair.h ../lib/types.h ../lib/vector.h
		gcc $(CFLAGS) -c maze.c -o maze.o
	
router.o: ../components/router.c ../components/router.h ../components/coordinate.h ../components/grid.h ../lib/queue.h ../lib/vector.h
		gcc $(CFLAGS) -c router.c -o router.o

list.o: ../lib/list.c ../lib/lib.h ../lib/types.h
		gcc $(CFLAGS)  -c list.c -o list.o

pair.o: ../lib/pair.c ../lib/pair.h 
		gcc $(CFLAGS)  -c pair.c -o pair.o

queue.o: ../lib/queue.c ../lib/queue.h ../lib/types.h
		gcc $(CFLAGS) -c queue.c -o queue.o

vector.o: ../lib/vector.c ../lib/vector.h ../lib/types.h ../lib/utility.h
		gcc $(CFLAGS) -c vector.c -o vector.o

clean: 
		rm -f *.o ../components/*.o ../lib/*.o CircuitRouter-SeqSolver 
