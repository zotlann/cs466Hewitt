#Compiler
CC=g++
#Compiler Flags
CXXFLAGS=-g -Wall -std=c++0x

# .o compilatoin
O_FILES=main.o config.o metadata.o

all: $(O_FILES)
	$(CC) $(CXXFLAGS) -o PA1 $(O_FILES)
main.o: PA1.cpp
	$(CC) $(CXXFLAGS) -c PA1.cpp -o main.o
config.o: config.cpp
	$(CC) $(CXXFLAGS) -c config.cpp -o config.o
metadeta.o: metadeta.cpp
	$(CC) $(CXXFLAGS) -c metadata.cpp -o metadata.o
