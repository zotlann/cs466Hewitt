#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "process.h"
#include <vector>

enum Scheduler {SJF,FIFO,PS};

std::vector<Process*> schedule(std::vector<Process*> processes, Scheduler scheduler);
std::vector<Process*> schedulePS(std::vector<Process*> processes);
std::vector<Process*> scheduleSJF(std::vector<Process*> processes);

#endif //SCHEDULER_H
