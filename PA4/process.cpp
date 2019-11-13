#include "process.h"

Process::Process(std::vector<Code*> ops, PCB* control){
	pcb = control;
	operations = ops;
	num_operations = ops.size();
	num_io_operations = 0;
	for(int i = 0; i < ops.size(); i++){
		if((ops[i]->getOperation() == 'I') || (ops[i]->getOperation() == 'O')){
			num_io_operations++;
		}
	}
}

PCB* Process::getPCB(){
	return pcb;
}

int Process::getNumIO(){
	return num_io_operations;
}

int Process::getNumOps(){
	return num_operations;
}

std::vector<Code*> Process::getOps(){
	return operations;
}
