EXENAME = main
OBJS = main.o

CXX = g++
CXXFLAGS = -std=c++14 -stdlib=libc++ -lssl -lcrypto -Wall -lboost_system

main.o: main.cpp
	$(CXX) $(CXXFLAGS) main.cpp -o $(OBJS)

clean:
	-rm -f *.o $(EXENAME)

