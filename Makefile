CXX = g++
CXXFLAGS = -std=c++11 -Iinclude

all: memsim

memsim: src/main.cpp
	$(CXX) $(CXXFLAGS) src/main.cpp -o memsim

run: memsim
	./memsim

clean:
	rm -f memsim

.PHONY: all clean run