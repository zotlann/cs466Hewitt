//--------------------------------------------------------------------
// Jonathon Hewitt
// config.cpp
// implementation of the Config class and related functions
//--------------------------------------------------------------------
#include "config.h"

//Default Constructor for Config
//This should almost never be used.
Config::Config(){
	metadata_filename = " ";
	version = " ";
	monitor_display_time = 0;
	processor_cycle_time = 0;
	mouse_cycle_time = 0;
	hdd_cycle_time = 0;
	keyboard_cycle_time = 0;
	memory_cycle_time = 0;
	printer_cycle_time = 0;
	log_to_file = false;
	log_to_console = false;
	log_filename = " ";
};

//Parameterized Consructor for Config that parses the config file
//Errors are thrown if the file is empty, nonexistant or does not have the .conf file extension.
//TODO add errors for incomplete config files e.g. not all necessary values are set.
Config::Config(std::string config_filename){
	std::string temp_string;
	std::string file_extension;
	std::string error;
	int temp_int;

	//get a substr starting after the last . in the file name
	//AKA get the file extension
	//If invalid file extension, throw error
	file_extension = config_filename.substr(config_filename.find_last_of('.') +1);
	if(file_extension != "conf"){
		error = "Invalid filename for config file: " + file_extension + "\n";
		throw std::logic_error(error);
	}

	//Open the file and check that it is good, if it's not throw an error
	std::ifstream config_file;
	config_file.open(config_filename.c_str());
	if(!config_file.good()){
		throw std::logic_error("Invalid config file path");
		return;
	}

	//Make sure the file is non empty, if it is throw an error
	if(config_file.peek() == std::ifstream::traits_type::eof()){
		throw std::logic_error("Config file is valid file, but empty\n");
	}

	//While not reading at end of file, parse the config file and set
	//appropriate data members
	//TODO add error handling for recieving unexpected values for some data members
	//e.g. error for recieving negative cycle times or missing cycle times or a string where an integer should be
	while(!config_file.eof()){
		//get first string of new line
		config_file >> temp_string;
		//if the first string is start, there is no data on this line
		//just go to next line
		if(temp_string == "Start"){
			std::getline(config_file,temp_string);
		}
		//if the first string is Version/Phase: set the version
		else if(temp_string == "Version/Phase:"){
			config_file >> temp_string;
			version = temp_string;
		}
		//if the first string is File, set the metadata filepath
		else if(temp_string == "File"){
			std::getline(config_file,temp_string,':');
			config_file >> temp_string;
			metadata_filename = temp_string;
		}
		//if the first string is Monitor, set the moitor display time
		else if(temp_string == "Monitor"){
			std::getline(config_file,temp_string,':');
			config_file >> temp_int;
			monitor_display_time = temp_int;
		}
		//if the first string is Processor, set the processor time
		else if(temp_string == "Processor"){
			std::getline(config_file,temp_string,':');
			config_file >> temp_int;
			processor_cycle_time = temp_int;
		}
		//if the first string is Mouse, set the mouse time
		else if(temp_string == "Mouse"){
			std::getline(config_file,temp_string,':');
			config_file >> temp_int;
			mouse_cycle_time = temp_int;
		}
		//if the first string is Hard, set the hdd time
		else if(temp_string == "Hard"){
			std::getline(config_file,temp_string,':');
			config_file >> temp_int;
			hdd_cycle_time = temp_int;
		}
		//if the first string is Keyboard, set the keyboard time
		else if(temp_string == "Keyboard"){
			std::getline(config_file,temp_string,':');
			config_file >> temp_int;
			keyboard_cycle_time = temp_int;
		}
		//if the first string is scanner, set the scanner time
		else if(temp_string == "Scanner"){
			std::getline(config_file,temp_string,':');
			config_file >> temp_int;
			scanner_cycle_time = temp_int;
		}
		//if the first string is Memory, set the memory time
		else if(temp_string == "Memory"){
			std::getline(config_file,temp_string,':');
			config_file >> temp_int;
			memory_cycle_time = temp_int;
		}
		//if the first string is Printer, set the printer time
		else if(temp_string == "Printer"){
			std::getline(config_file,temp_string,':');
			config_file >> temp_int;
			printer_cycle_time = temp_int;
		}
		//if the first string is Projector, set the projector time
		else if(temp_string == "Projector"){
			std::getline(config_file,temp_string,':');
			config_file >> temp_int;
			projector_cycle_time = temp_int;
		}
		//if the first string is system, set the memory size and system memory
		else if(temp_string == "System"){
			std::getline(config_file,temp_string,'{');
			std::getline(config_file,temp_string,'}');
			memory_size = temp_string;
			std::getline(config_file,temp_string,':');
			config_file >> temp_int;
			system_memory = temp_int;
		}
		//if the first string is Log: set the log boolans
		else if(temp_string == "Log:"){
			config_file >> temp_string;
			config_file >> temp_string;
			config_file >> temp_string;
			//if log to both set both to true
			if(temp_string == "Both"){
				log_to_console = true;
				log_to_file = true;
			}
			//if log to file
			else if(temp_string == "File"){
				log_to_console = false;
				log_to_file = true;
			}
			//if log to monitor
			else if(temp_string == "Monitor"){
				log_to_console = true;
				log_to_file = false;
			}
		}
		//if the first string is Log set the log file path
		else if(temp_string == "Log"){
			std::getline(config_file,temp_string,':');
			config_file >> temp_string;
			log_filename = temp_string;
		}
		//if the first string is End, stop parsing config file
		else if(temp_string == "End"){
			config_file.close();
			return;
		}
	}
	//delete contents of log file if it exists
	std::ofstream log_file;
	log_file.open(getLogFilename().c_str(),std::ofstream::trunc);
	log_file.close();

}

//Getters and Setters for Config data members
std::string Config::getMetadataFilename(){
	return metadata_filename;
}
void Config::setMetadataFilename(std::string s){
	metadata_filename = s;
}
std::string Config::getVersion(){
	return version;
}
void Config::setVersion(std::string s){
	version = s;
}
int Config::getMonitorDisplayTime(){
	return monitor_display_time;
}
void Config::setMonitorDisplayTime(int n){
	monitor_display_time = n;
}
int Config::getProcessorCycleTime(){
	return processor_cycle_time;
}
void Config::setProcessorCycleTime(int n){
	processor_cycle_time = n;
}
int Config::getMouseCycleTime(){
	return mouse_cycle_time;
}
void Config::setMouseCycleTime(int n){
	mouse_cycle_time = n;
}
int Config::getHDDCycleTime(){
	return hdd_cycle_time;
}
void Config::setHDDCycleTime(int n){
	hdd_cycle_time = n;
}
int Config::getKeyboardCycleTime(){
	return keyboard_cycle_time;
}
void Config::setKeyboardCycleTime(int n){
	keyboard_cycle_time = n;
}
int Config::getScannerCycleTime(){
	return scanner_cycle_time;
}
void Config::setScannerCycleTime(int n){
	scanner_cycle_time = n;
}
int Config::getMemoryCycleTime(){
	return memory_cycle_time;
}
void Config::setMemoryCycleTime(int n){
	memory_cycle_time = n;
}
int Config::getPrinterCycleTime(){
	return printer_cycle_time;
}
void Config::setPrinterCycleTime(int n){
	printer_cycle_time = n;
}
int Config::getProjectorCycleTime(){
	return projector_cycle_time;
}
void Config::setProjectorCycleTime(int n){
	projector_cycle_time = n;
}
int Config::getSystemMemory(){
	return system_memory;
}
void Config::setSystemMemory(int n){
	system_memory = n;
}
std::string Config::getMemorySize(){
	return memory_size;
}
void Config::setMemorySize(std::string s){
	memory_size = s;
}
bool Config::logToFile(){
	return log_to_file;
}
void Config::setLogToFile(bool b){
	log_to_file = b;
}
bool Config::logToConsole(){
	return log_to_console;
}
void Config::setLogToConsole(bool b){
	log_to_console = b;
}
std::string Config::getLogFilename(){
	return log_filename;
}
void Config::setLogFilename(std::string s){
	log_filename = s;
}

//<< Operator overload for outputting config data
//Format taken from example output in homework specs.
std::ostream& operator<<(std::ostream& os, Config cfg){
	os << "Configuration File Data\n";
	os << "Monitor = " << cfg.getMonitorDisplayTime() << " ms/cycle\n";
	os << "Processor = " << cfg.getProcessorCycleTime() << " ms/cycle\n";
	os << "Mouse = " << cfg.getMouseCycleTime() << " ms/cycle\n";
	os << "Hard Drive = " << cfg.getHDDCycleTime() << " ms/cycle\n";
	os << "Keyboard = " << cfg.getKeyboardCycleTime() << " ms/cycle\n";
	os << "Memory = " << cfg.getMemoryCycleTime() << " ms/cycle\n";
	os << "Printer = " << cfg.getPrinterCycleTime() << " ms/cycle\n";
	os << "Logged to: ";
	if(cfg.logToConsole() && cfg.logToFile()){
		os << "monitor and " << cfg.getLogFilename() << std::endl;
	}
	else if(cfg.logToConsole()){
		os << "monitor" << std::endl;
	}
	else if(cfg.logToFile()){
		os << cfg.getLogFilename() << std::endl;
	}
	os << "\n";
	return os;
}
		
//Log Function
//Logs to File/Console depending on what was set in the config flie
void Config::log(){
	std::ofstream log_file;
	if(logToFile()){
		log_file.open(getLogFilename().c_str(),std::ofstream::trunc);
		log_file << *this;
		log_file.close();
	}
	if(logToConsole()){
		std::cout << *this;
	}
}
