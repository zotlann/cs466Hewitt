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
#include "resources.h"
#include <vector>
#include <sstream>
#include <algorithm>
#include <chrono>
#include <ctime>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include "process.h"
#include "code.h"

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
		void runProcess(Process* process, pthread_t, std::chrono::high_resolution_clock::time_point);
		std::string runOperation(Code* code, pthread_t thread_id, std::chrono::high_resolution_clock::time_point tp, PCB* pcb);
		

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
		std::vector<Process*> processes;

		//I/O resource handler
		Resources* resources;

		unsigned int next_memory_block;

		//semaphores and mutexes
		sem_t projector_sem;
		sem_t hdd_sem;
		sem_t scanner_sem;
		sem_t monitor_sem;
		sem_t printer_sem;
		sem_t keyboard_sem;
		sem_t mouse_sem;

		pthread_mutex_t projector_lock;
		pthread_mutex_t hdd_lock;
		pthread_mutex_t scanner_lock;
		pthread_mutex_t monitor_lock;
		pthread_mutex_t printer_lock;
		pthread_mutex_t keyboard_lock;
		pthread_mutex_t mouse_lock;

		std::vector<Code*> codes;
};

//read metadata file and build the vector of codes
std::vector<Code*> parseMetadataFile(std::string);
#endif //METADATA_H
