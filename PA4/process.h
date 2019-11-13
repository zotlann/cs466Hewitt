#ifndef PROCESS_H
#define PROCESS_H

#include "pcb.h"
#include "code.h"
#include <vector>

class Process{
	public:
		Process(std::vector<Code*> ops, PCB* control);
		PCB* getPCB();
		int getNumIO();
		int getNumOps();
		std::vector<Code*> getOps();
	private:
		int num_io_operations;
		int num_operations;
		std::vector<Code*> operations;
		PCB* pcb;
};

#endif //PROCESS_H

