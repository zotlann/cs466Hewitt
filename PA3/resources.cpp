//--------------------------------------------------------------------
// Jonathon Hewitt
// resources.cpp
// Implementation of the Resources class and related methods
//--------------------------------------------------------------------

#include "resources.h"
Resources::Resources(Config cfg){
	for(int i = 0; i < cfg.getHDDQuantity(); i++){
		hdd_deque.push_back(i);
	}
	for(int i = 0; i < cfg.getKeyboardQuantity(); i++){
		keyboard_deque.push_back(i);
	}
	for(int i = 0; i < cfg.getMonitorQuantity(); i++){
		monitor_deque.push_back(i);
	}
	for(int i = 0; i < cfg.getScannerQuantity(); i++){
		scanner_deque.push_back(i);
	}
	for(int i = 0; i < cfg.getProjectorQuantity(); i++){
		projector_deque.push_back(i);
	}
	for(int i = 0; i < cfg.getPrinterQuantity(); i++){
		printer_deque.push_back(i);
	}
	for(int i = 0; i < cfg.getMouseQuantity(); i++){
		mouse_deque.push_back(i);
	}
}

void Resources::manageDevice(Device device){
	int temp_int;
	switch(device){
		case HDD:
			temp_int = hdd_deque.front();
			hdd_deque.pop_front();
			hdd_deque.push_back(temp_int);
			break;
		case KEYBOARD:
			temp_int = keyboard_deque.front();
			keyboard_deque.pop_front();
			keyboard_deque.push_back(temp_int);
			break;
		case MONITOR:
			temp_int = monitor_deque.front();
			monitor_deque.pop_front();
			monitor_deque.push_back(temp_int);
			break;
		case SCANNER:
			temp_int = scanner_deque.front();
			scanner_deque.pop_front();
			scanner_deque.push_back(temp_int);
			break;
		case PROJECTOR:
			temp_int = projector_deque.front();
			projector_deque.pop_front();
			projector_deque.push_back(temp_int);
			break;
		case PRINTER:
			temp_int = printer_deque.front();
			printer_deque.pop_front();
			printer_deque.push_back(temp_int);
			break;
		case MOUSE:
			temp_int = mouse_deque.front();
			printer_deque.pop_front();
			printer_deque.push_back(temp_int);
			break;

	}
}

