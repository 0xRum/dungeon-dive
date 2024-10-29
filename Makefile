
main: main.o roomManip.o stringManip.o
	gcc -o main main.o roomManip.o stringManip.o

main.o: main.c roomManip.h stringManip.h
	gcc -c main.c -o main.o

roomManip.o: roomManip.c roomManip.h stringManip.h
	gcc -c roomManip.c -o roomManip.o

stringManip.o: stringManip.c stringManip.h
	gcc -c stringManip.c -o stringManip.o

clean:
	rm -f *.o main
