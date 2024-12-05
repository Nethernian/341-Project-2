cCXX = g++
CXXFLAGS = -Wall -g

mytest: utree.o dtree.o driver.cpp
	$(CXX) $(CXXFLAGS) dtree.o utree.o driver.cpp -o mytest

utree.o: utree.h utree.cpp dtree.o
	$(CXX) $(CXXFLAGS) -c dtree.o utree.cpp

dtree.o: dtree.h dtree.cpp
	$(CXX) $(CXXFLAGS) -c dtree.cpp

run:
	./mytest

