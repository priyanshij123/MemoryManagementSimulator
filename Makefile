CXX = g++
CXXFLAGS = -std=c++11 -Iinclude

all:
	$(CXX) $(CXXFLAGS) src/main.cpp -o memsim

clean:
	rm -f memsim
