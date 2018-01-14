test: test.o util.o base.o
	gcc -o test test.o util.o base.o

test.o: test.c
	gcc -c test.c
chessi.o: chessi.c
	gcc -c chessi.c
util.o: util.c
	gcc -c util.c
base.o: base.c
	gcc -c base.c