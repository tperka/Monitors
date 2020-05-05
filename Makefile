runner.out: buffer.o main.o
	g++ buffer.o main.o -pthread -o runner.out

buffer.o:
	g++ buffer.cpp -c

main.o:
	g++ main.cpp -c

clean:
	rm -f *.o *.out *~ .*~
