CC = g++

CompileParms = -c -Wall -std=c++11 -O2

OBJS = standaard.o stand.o main.o

Opdr: $(OBJS)
	$(CC) $(OBJS) -o Spel

clean:
	rm -f *.o Spel

standaard.o: standaard.cc standaard.h
	$(CC) $(CompileParms)  standaard.cc

stand.o: stand.cc standaard.h constantes.h stand.h
	$(CC) $(CompileParms)  stand.cc

main.o: main.cc standaard.h constantes.h stand.h
	$(CC) $(CompileParms)  main.cc

