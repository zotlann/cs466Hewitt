//--------------------------------------------------------------------
// Jonathon Hewitt
// utility.cpp
// Implementations of utility functions that might be useful in other
// files
//--------------------------------------------------------------------
#include "utility.h"

//wrapper for the wait function so that it can be used with pthreads
void *threadTimer(void* wait_time){
	wait(*(int*)wait_time);
	return 0;
}
//waits for wait_time_milli miliseconds, using unix sleep function
void wait(int wait_time_mili){
	unsigned int wait_time_micro = wait_time_mili * 1000;
	usleep(wait_time_micro);
}

//retuns the timestamp in a nice format for printing
std::string getTimeStamp(std::chrono::high_resolution_clock::time_point tp){
	return std::to_string(std::chrono::duration_cast<std::chrono::duration<double>>( std::chrono::high_resolution_clock::now() - tp ).count());
}
