CC = g++

OPT = -Wall -Wextra -ansi -pedantic -g
LIB = 
OBJ = main.o data.o solution.o stats.o

exe: $(OBJ)
	$(CC) $(OBJ) -o exe $(LIB)

main.o: main.cpp data.hpp
	$(CC) -c main.cpp $(OPT)

data.o: data.cpp data.hpp
	$(CC) -c data.cpp $(OPT)

solution.o: solution.cpp solution.hpp
	$(CC) -c solution.cpp $(OPT)

stats.o: stats.cpp stats.hpp
	$(CC) -c stats.cpp $(OPT)

clean:
	rm $(OBJ)
