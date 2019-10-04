//--------------------------------------------------------------------
// Jonathon Hewitt
// metadata.h
// Declaration of the Code and MetaData classes along with relevant
// functions.
// These classes will hold all relevant data from the metadata code
// files
//--------------------------------------------------------------------

#ifndef METADATA_H
#define METADATA_H

#include "config.h"
#include "pcb.h"
#include "utility.h"
#include <vector>
#include <sstream>
#include <algorithm>
#include <chrono>
#include <ctime>
#include <time.h>
#include <unistd.h>
#include <pthread.h>

//Code class which holds individual operations and their associated data
class Code{
	public:
		//Constructors
		Code();
		Code(std::string);

		//Getters and setters
		char getOperation();
		void setOperation(char);

		std::string getDescription();
		void setDescription(std::string);

		int getCycles();
		void setCycles(int);
		
		//whether or not a code should be output and logged
		bool logCode();

		//format the code in a nice print friendly way
		friend std::ostream& operator<<(std::ostream& os,Code code);
	private:
		char operation;
		std::string description;
		int cycles;
};

//MetaData class which holds a vector of Codes, along with associated config data and PCB for 
//the program
class MetaData{
	public:
		//Constructors
		MetaData();
		MetaData(Config);

		//Getters and setters
		Config getConfig();
		void setConfig(Config);

		std::vector<Code*> getCodes();
		void setCodes(std::vector<Code*>);
	
		//allocates a block of memory at a random valid address
		std::string allocateMemory();

		//run the progam and output time data
		void run();
		std::string runProcess(Code* code, pthread_t thread_id, std::chrono::high_resolution_clock::time_point tp);

		//get the number of miliseconds a section of code should run based on number of 
		//cycles and config settings
		int getCodeTime(Code);

		//log the entire metadata program to appropriate channels
		void log();

		//log the given string to appropriate channels
		void log(std::string s);

		//format and output the metadata code in a nice format
		friend std::ostream& operator<<(std::ostream& os,MetaData metadata);
	private:
		Config config;
		std::vector<Code*> codes;
		PCB pcb;
};

//read metadata file and build the vector of codes
std::vector<Code*> parseMetadataFile(std::string);
#endif //METADATA_H
