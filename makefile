CPPFLAGS= -Wall -Wextra -pedantic -g

all:	Test_Array

Test_Array:	Test_Array.o
				g++ $(CPPFLAGS) -pthread Test_Array.o -o Test_Array

Test_Array.o:	Test_Array.cpp
					g++ -std=c++11 $(CPPFLAGS) -pthread -c Test_Array.cpp

run:	Test_Array
		./Test_Array

checkmem:	Test_Array
			valgrind --leak-check=full ./Test_Array

clean:
			rm -f *.o Test_Array
