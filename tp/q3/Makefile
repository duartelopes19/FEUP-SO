all : cypher

cypher : cypher.o
	gcc -o cypher cypher.o

cypher.o : cypher.c
	gcc -c cypher.c

clean :
	rm cypher *.o
