//--------------------------------------------------------------------
// Jonathon Hewitt
// utility.h
// declarations for utility functions that may be useful throughout 
// other files.  
// -------------------------------------------------------------------

#ifndef UTILITY_H
#define UTILITY_H

#include <chrono>
#include <unistd.h>
#include <string>
#include <sstream>

//void *threadTimer(void* wait_time);
//function pointer for when using pthread
//wait_time is a pointer to an integer for how long wait in miliseconds
void *threadTimer(void* wait_time);

//void wait(int)
//waits for wait_time_mili miliseconds
void wait(int wait_time_mili);

//returns a time stamp in a nicely formatted string for printing
std::string getTimeStamp(std::chrono::high_resolution_clock::time_point tp);

#endif //UTILITY_H

