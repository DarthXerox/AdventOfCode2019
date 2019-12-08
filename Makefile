CXXFLAGS = -Wall -std=c++11 -Wextra
all: clean AoC

AoC: main.o Days.o
	g++ main.o -o advent

main.o: main.cpp
Days.o: Days.cpp

clean:
	$(RM) *.o advent

.PHONY: clean all

