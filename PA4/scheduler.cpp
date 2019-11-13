#include "scheduler.h"

std::vector<Process*> schedule(std::vector<Process*> processes, Scheduler scheduler){
	std::cout << std::endl;
	switch(scheduler){
		case FIFO:
			return processes;
			break;
		case SJF:
			return scheduleSJF(processes);
			break;
		case PS:
			return schedulePS(processes);
			break;
		}
}

std::vector<Process*> schedulePS(std::vector<Process*> processes){
	for(int i = 0; i < processes.size() - 1; i++){
		for(int j = 0; j < processes.size() - i - 1; j++){
			if(processes[j]->getNumIO() > processes[j+1]->getNumIO()){
					std::swap(processes[j],processes[j+1]);
			}
		}
	}
	return processes;
}

std::vector<Process*> scheduleSJF(std::vector<Process*> processes){
	for(int i = 0; i < processes.size() - 1; i++){
		for(int j = 0; j < processes.size() - i - 1; j++){
			if(processes[j]->getNumOps() > processes[j+1]->getNumOps()){
					std::swap(processes[j],processes[j+1]);
			}
		}
	}
	return processes;
}
