all: server.out client.out
	echo "Build done!"

server.out: server.o func.o
	gcc server.o func.o -o server.out -pthread

client.out: client.o func.o
	gcc client.o func.o -o client.out

server.o: server.c func.h
	gcc -c server.c

client.o: client.c func.h
	gcc -c client.c

func.o: func.c func.h
	gcc -c func.c

clean:
	rm -f *.o *.out