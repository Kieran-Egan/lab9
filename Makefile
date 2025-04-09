CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra

all: main

main: main.cpp
	$(CXX) $(CXXFLAGS) main.cpp -o main

clean:
	rm -f main
