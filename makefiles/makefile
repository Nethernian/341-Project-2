cCXX = g++
CXXFLAGS = -Wall -g

mytest: dtree.o utree.o mytest.cpp
	$(CXX) $(CXXFLAGS) buffer.o bufferlist.o mytest.cpp -o mytest

utree.o: dtree.o utree.h utree.cpp
	$(CXX) $(CXXFLAGS) -c bufferlist.cpp

dtree.o: dtree.h dtree.cpp
	$(CXX) $(CXXFLAGS) -c buffer.cpp

run:
	./mytest

