OBJS	= main.o hash_table.o list.o
SOURCE	= main.cpp hash_table.cpp list.cpp
HEADER	= hash_table.hpp list.hpp semaphores_sheared_mem.hpp
OUT	= main
CC	 = g++
FLAGS	 = -g -c -Wall -g3
LFLAGS	 =

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

main.o: main.cpp
	$(CC) $(FLAGS) main.cpp -std=c11

hash_table.o: hash_table.cpp
	$(CC) $(FLAGS) hash_table.cpp -std=c11

list.o: list.cpp
	$(CC) $(FLAGS) list.cpp -std=c11


clean:
	rm -f $(OBJS) $(OUT)
