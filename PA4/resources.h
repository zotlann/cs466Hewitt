//--------------------------------------------------------------------
// Jonathon Hewitt
// resources.h
// Declaration of the Resources class
// This class is responsible for managing the resources of multiple
// I/O devices
//--------------------------------------------------------------------

#ifndef RESOURCES_H
#define RESOURCES_H

#include "config.h"
#include <deque>

enum Device{HDD,KEYBOARD,MONITOR,SCANNER,PROJECTOR,PRINTER,MOUSE};

class Resources{
	public:
		Resources(Config cfg);
		void manageDevice(Device device);
		std::deque<int> hdd_deque;
		std::deque<int> keyboard_deque;
		std::deque<int> monitor_deque;
		std::deque<int> scanner_deque;
		std::deque<int> projector_deque;
		std::deque<int> printer_deque;
		std::deque<int> mouse_deque;
};



#endif //RESOURCES_H
