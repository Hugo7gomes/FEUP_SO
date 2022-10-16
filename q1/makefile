#This is a makefile

all: phrases

phrases: phrases.c
	gcc -Wall -o phrases phrases.c

run:
	./phrases quote.txt
	./phrases -l quote.txt

clean:
	rm -rf phrases
	rm -rf *.exe
	rm -rf *.o
