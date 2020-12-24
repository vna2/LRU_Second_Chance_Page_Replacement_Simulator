OBJS	= main.o hash_table.o list.o hash_function.o
SOURCE	= main.cpp hash_table.cpp list.cpp hash_function.cpp
HEADER	= hash_table.hpp list.hpp semaphores_sheared_mem.hpp hash_function.hpp
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

hash_function.o: hash_function.cpp
	$(CC) $(FLAGS) hash_function.cpp -std=c11

clean:
	rm -f $(OBJS) $(OUT)
