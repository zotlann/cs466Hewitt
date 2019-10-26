//--------------------------------------------------------------------
// Jonathon Hewitt
// config.h
// Declaration of the Config class
// This class will hold all of the relevant OS and Hardware 
// configuration data
//--------------------------------------------------------------------

#ifndef CONFIG_H
#define CONFIG_H

#include <iostream>
#include <fstream>
#include <string>
#include <exception>

class Config{
	public:
		//Constructors
		Config();
		Config(std::string);

		//Getters and Setters
		std::string getMetadataFilename();
		void setMetadataFilename(std::string);

		std::string getVersion();
		void setVersion(std::string);

		int getMonitorDisplayTime();
		void setMonitorDisplayTime(int);

		int getProcessorCycleTime();
		void setProcessorCycleTime(int);

		int getMouseCycleTime();
		void setMouseCycleTime(int);

		int getHDDCycleTime();
		void setHDDCycleTime(int);

		int getKeyboardCycleTime();
		void setKeyboardCycleTime(int);

		int getScannerCycleTime();
		void setScannerCycleTime(int);
		
		int getProjectorCycleTime();
		void setProjectorCycleTime(int);

		int getMemoryCycleTime();
		void setMemoryCycleTime(int);

		int getPrinterCycleTime();
		void setPrinterCycleTime(int);

		int getSystemMemory();
		void setSystemMemory(int);
		
		std::string getMemorySize();
		void setMemorySize(std::string);

		bool logToFile();
		void setLogToFile(bool);

		bool logToConsole();
		void setLogToConsole(bool);

		std::string getLogFilename();
		void setLogFilename(std::string);

		int getHDDQuantity();
		void setHDDQuantity(int);

		int getPrinterQuantity();
		void setPrinterQuantity(int);

		int getKeyboardQuantity();
		void setKeyboardQuantity(int);

		int getMonitorQuantity();
		void setMonitorQuantity(int);

		int getScannerQuantity();
		void setScannerQuantity(int);

		int getProjectorQuantity();
		void setProjectorQuantity(int);

		int getMouseQuantity();
		void setMouseQuantity(int);
		
		int getBlockSize();

		//Appropriately logs config data
		void log();

		//Prints the config data in a nice format
		friend std::ostream& operator<<(std::ostream& os, Config cfg);
	
	private:
		std::string metadata_filename;
		std::string version;
		std::string memory_size;
		int monitor_display_time;
		int processor_cycle_time;
		int mouse_cycle_time;
		int hdd_cycle_time;
		int keyboard_cycle_time;
		int scanner_cycle_time;
		int memory_cycle_time;
		int printer_cycle_time;
		int projector_cycle_time;
		int printer_quantity;
		int hdd_quantity;
		int monitor_quantity;
		int scanner_quantity;
		int keyboard_quantity;
		int projector_quantity;
		int mouse_quantity;
		int memory_block_size;
		int system_memory;

		bool log_to_file;
		bool log_to_console;

		std::string log_filename;
};

#endif //CONFIG_H
