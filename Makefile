CFLAGS = -O
CC = g++
output: main.o node.o utils.o events.o
	$(CC) $(CFLAGS) -o output main.o node.o utils.o events.o
main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp
node.o: node.cpp
	$(CC) $(CFLAGS) -c node.cpp
utils.o: utils.cpp
	$(CC) $(CFLAGS) -c utils.cpp
events.o: events.cpp
	$(CC) $(CFLAGS) -c events.cpp
clean:
	rm -f *.o output *.txt
