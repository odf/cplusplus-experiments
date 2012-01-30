test:	test.o
	g++ test.o -o test
	./test

# DO NOT DELETE

test.o: LinkedList.hpp LazyList.hpp
