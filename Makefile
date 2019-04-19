CXXFLAGS = -std=c++11 -g -Wall -Wextra -Werror

bloom: bloom.o BloomFilter.o HashSet.o
	${CXX} $(CXXFLAGS) -o $@ $^

bloom.o: bloom.cpp | BloomFilter.h HashSet.h
	${CXX} $(CXXFLAGS) -c -o $@ $^

BloomFilter.o: BloomFilter.cpp | BloomFilter.h HashFunction.h
	${CXX} $(CXXFLAGS) -c -o $@ $^

HashFunctions.o: HashFunctions.cpp | HashFunctions.h
	${CXX} $(CXXFLAGS) -c -o $@ $^

HashSet.o: HashSet.cpp | HashFunction.h HashSet.h
	${CXX} $(CXXFLAGS) -c -o $@ $^

clean:
	rm -rf bloom *.o
