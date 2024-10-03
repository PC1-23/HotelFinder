hotelFinder: main.o hotel.o hashtable.o 
	g++ main.o hotel.o hashtable.o -o hotelFinder
main.o: main.cpp
	g++ -c main.cpp
hotel.o: hotel.hpp hotel.cpp bst.hpp
	g++ -c hotel.cpp 
hashtable.o: hashtable.hpp hashtable.cpp bst.hpp
	g++ -c hashtable.cpp
clean:
	rm *.o hotelFinder

