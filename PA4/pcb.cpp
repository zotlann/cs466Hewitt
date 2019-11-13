#include "pcb.h"

//Default PCB  constructor
PCB::PCB(){
	id = 0;
	state = DEFAULT;
}

//Parameterized PCB constructor
PCB::PCB(int pid, PCBState pstate){
	id = pid;
	state = pstate;
}

//Getters and setters
int PCB::getID(){
	return id;
}

PCBState PCB::getState(){
	return state;
}

void PCB::setID(int pid){
	id = pid;
}

void PCB::setState(PCBState pstate){
	state = pstate;
}
