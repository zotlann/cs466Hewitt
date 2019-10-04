#Compiler
CC=g++
#Compiler Flags
CXXFLAGS=-g -Wall -std=c++0x -lpthread

# .o compilatoin
O_FILES=main.o config.o metadata.o pcb.o utility.o

all: $(O_FILES)
	$(CC) $(CXXFLAGS) -o PA2 $(O_FILES)
main.o: PA2.cpp
	$(CC) $(CXXFLAGS) -c PA2.cpp -o main.o
config.o: config.cpp
	$(CC) $(CXXFLAGS) -c config.cpp -o config.o
metadeta.o: metadeta.cpp
	$(CC) $(CXXFLAGS) -c metadata.cpp -o metadata.o
pcb.o: pcb.cpp
	$(CC) $(CXXFLAGS) -c pcb.cpp -o pcb.o
utility.o: utility.cpp
	$(CC) $(CXXFLAGS) -c utility.cpp -o utility.o
