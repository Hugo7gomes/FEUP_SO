#This is a makefile

all: cypher

phrases: cypher.c
	gcc -Wall -o cypher cypher.c

run:
	./cypher < quote.txt > quote_cyphered.txt

clean:
	rm -rf cypher
	rm -rf quote_cyphered.txt
	rm -rf *.exe
	rm -rf *.o
