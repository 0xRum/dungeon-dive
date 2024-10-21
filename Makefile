all: main

main: main.o roomManip.o stringManip.o
	gcc -o main main.o roomManip.o stringManip.o

main.o: main.c roomManip.h stringManip.h
	gcc -c main.c

roomManip.o: roomManip.c roomManip.h stringManip.h
	gcc -c roomManip.c

stringManip.o: stringManip.c stringManip.h
	gcc -c stringManip.c

clean:
	rm -f *.o main
