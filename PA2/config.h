#ifndef CONFIG_H
#define CONFIG_H

#include <iostream>
#include <fstream>
#include <string>
#include <exception>

class Config{
	public:
		Config();
		Config(std::string);

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

		int getMemoryCycleTime();
		void setMemoryCycleTime(int);

		int getPrinterCycleTime();
		void setPrinterCycleTime(int);

		bool logToFile();
		void setLogToFile(bool);

		bool logToConsole();
		void setLogToConsole(bool);

		std::string getLogFilename();
		void setLogFilename(std::string);

		void log();

		friend std::ostream& operator<<(std::ostream& os, Config cfg);
	private:
		std::string metadata_filename;
		std::string version;
		
		int monitor_display_time;
		int processor_cycle_time;
		int mouse_cycle_time;
		int hdd_cycle_time;
		int keyboard_cycle_time;
		int memory_cycle_time;
		int printer_cycle_time;

		bool log_to_file;
		bool log_to_console;

		std::string log_filename;
};

#endif //CONFIG_H
