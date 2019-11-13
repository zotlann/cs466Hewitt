//--------------------------------------------------------------------
// Jonathon Hewitt
// metadata.cpp
// Implementation of the Code and MetaData classes along with 
// relevant functions
//--------------------------------------------------------------------

#include "metadata.h"
#include <chrono>

pthread_mutex_t mutex;
sem_t mutexes[2];

//Default Constructor for Code
//this should almost never be used
Code::Code(){
	operation = '\0';
	description = " ";
	cycles = 0;
}

//Parameterized Constructor for Code
//String should be in the valid code form:
//"O{description}cycles;"
//where O is a single character representing the operation.
//valid operations are "S,A,P,I,O,M"
//valid descriptions are "begin, finish, hard drive, keyboard, mouse, monitor, run, allocate, printer, block"
//cycles can be any integer
//Example valid code
//S{run}10;
//NOTE: Whitespace is ignored, S    {   run } 10   ;
//gets transformed to S{run}10;
Code::Code(std::string code_string){
	//remove whitespace from string,putting it in an easy format to parse
	code_string.erase(std::remove(code_string.begin(),code_string.end(),' '),code_string.end());

	char temp_char;
	std::string temp_string;
	int temp_int;
	std::string error;

	//used to easily extract members from the string
	std::stringstream code_stream;

	//intialize code_stream to contain the code string
	code_stream << code_string;
	
	//get the operations from the string stream
	code_stream.get(temp_char);
	
	//if the operation is not a valid operation throw an error
	if((temp_char != 'S') && (temp_char != 'A') && (temp_char != 'P') && (temp_char != 'I') && (temp_char != 'O') && (temp_char != 'M')){
		error = "Invalid operation in metadata file: ";
		error.append(1,temp_char);
		error += "\n";
		throw std::logic_error(error);
	}
	operation = temp_char;

	//get the description from the string stream
	code_stream.get(temp_char);
	std::getline(code_stream,temp_string,'}');

	//if the operation is not a valid operation throw an error
	if((temp_string != "begin") &&(temp_string != "finish") && (temp_string != "harddrive") && (temp_string != "keyboard") && (temp_string != "mouse") && (temp_string != "monitor") && (temp_string != "scanner") && (temp_string != "projector") && (temp_string != "run") && (temp_string != "allocate") && (temp_string != "printer") && (temp_string != "block")){
		error = "Invalid descriptor: " + temp_string + "\n";
		throw std::logic_error(error);
		return;
	}

	//add the whitespace back for hard drive
	if(temp_string == "harddrive"){
		temp_string = "hard drive";
	}
	description = temp_string;

	//get the number of cycles
	code_stream >> temp_string;
	if(!isdigit(temp_string[0])){
		error = "Missing Cycle number before:	" + temp_string + "\n";
		throw std::logic_error(error);
	}
	sscanf(temp_string.c_str(),"%d",&temp_int);
	if(temp_int < 0){
		error = "Invalid cycle, cycles must be non-negative integers: " + std::to_string(temp_int);
		throw std::logic_error(error);
	}
	cycles = temp_int;
}

//Getters and Setters for Code Class
char Code::getOperation(){
	return operation;
}
void Code::setOperation(char c){
	operation = c;
}
std::string Code::getDescription(){
	return description;
}
void Code::setDescription(std::string s){
	description = s;
}
int Code::getCycles(){
	return cycles;
}
void Code::setCycles(int n){
	cycles = n;
}
bool Code::logCode(){
	if((operation == 'A') || (operation == 'S')){
		return false;
	}
	return true;
}

//overloaded << operator for Code class
std::ostream& operator<<(std::ostream& os, Code code){
	if(('S' == code.getOperation()) || 'A' == code.getOperation()){
		return os;
	}
	os << code.getOperation() << "{" << code.getDescription() << "}" << code.getCycles();
	return os;
}

//Default Constructor for MetaData class
//This should almost never be used
MetaData::MetaData(){
	Config cfg;
	std::vector<Code*> cds;
	config = cfg;
	codes = cds;
}

//Parameterized Constructor for MetaData class
MetaData::MetaData(Config cfg){ 
	config = cfg;
	codes = parseMetadataFile(config.getMetadataFilename());
	int j = 0; 
	int temp;
	processes.clear();
	for(int i = 0; i < codes.size(); i++){
		if(codes[i]->getOperation() == 'A'){
			std::vector<Code*> temp_codes;
			temp_codes.clear();
			temp_codes.push_back(codes[i]);
			for(int k = i+1; k < codes.size(); k++){
				temp_codes.push_back(codes[k]);
				if(codes[k]->getOperation() == 'A'){
					PCB* pcb = new PCB;
					pcb->setID(j);
					j++;
					pcb->setState(START);
					Process* p = new Process(temp_codes,pcb);
					processes.push_back(p);
					i = k;
					k = codes.size() + 1;
				}
					
			}
		}
		
	}	
	resources = new Resources(config);
	next_memory_block = 0;
	//initialize semaphores and mutexes
	pthread_mutex_init(&projector_lock,NULL);
	pthread_mutex_init(&hdd_lock,NULL);
	pthread_mutex_init(&monitor_lock,NULL);
	pthread_mutex_init(&keyboard_lock,NULL);
	pthread_mutex_init(&scanner_lock,NULL);
	pthread_mutex_init(&printer_lock,NULL);
	pthread_mutex_init(&mouse_lock,NULL);

	sem_init(&projector_sem,0,config.getProjectorQuantity());
	sem_init(&hdd_sem,0,config.getHDDQuantity());
	sem_init(&monitor_sem,0,config.getMonitorQuantity());
	sem_init(&keyboard_sem,0,config.getKeyboardQuantity());
	sem_init(&scanner_sem,0,config.getScannerQuantity());
	sem_init(&printer_sem,0,config.getPrinterQuantity());
	sem_init(&mouse_sem,0,config.getMouseQuantity());
}
//Getters and Setters for MetaData class data members
Config MetaData::getConfig(){
	return config;
}
void MetaData::setConfig(Config cfg){
	config = cfg;
}
std::vector<Code*> MetaData::getCodes(){
	return codes;
}
void MetaData::setCodes(std::vector<Code*> cds){
	codes = cds;
}
int MetaData::getCodeTime(Code code){
	if((code.getOperation() == 'A') || (code.getOperation() == 'S')){
		return 0;
	}
	if(code.getOperation() == 'P'){
		return config.getProcessorCycleTime() * code.getCycles();
	}
	else if(code.getOperation() == 'M'){
		return config.getMemoryCycleTime() * code.getCycles();
	}
	else{
		if(code.getDescription() == "hard drive"){
			return config.getHDDCycleTime() * code.getCycles();
		}
		else if(code.getDescription() == "keyboard"){
			return config.getKeyboardCycleTime() * code.getCycles();
		}
		else if(code.getDescription() == "mouse"){
			return config.getMouseCycleTime() * code.getCycles();
		}
		else if(code.getDescription() == "monitor"){
			return config.getMonitorDisplayTime() * code.getCycles();
		}
		else if(code.getDescription() == "printer"){
			return config.getPrinterCycleTime() * code.getCycles();
		}
	}
	return 0;
}

//run the program and output time data to appropriate channels
void MetaData::run(){
	pthread_t thread_id;
	std::chrono::high_resolution_clock::time_point tp;
	tp = std::chrono::high_resolution_clock::now();
	
	log(runOperation(codes[0],thread_id,tp,NULL));
	processes = schedule(processes, config.getScheduler());
	for(int i = 0; i < processes.size(); i++){
		runProcess(processes[i],thread_id,tp);
	}
	log(runOperation(codes[codes.size()-1],thread_id,tp,NULL));
}

void MetaData::runProcess(Process* process,pthread_t thread_id, std::chrono::high_resolution_clock::time_point tp){
	std::string log_string;

	std::vector<Code*> operations = process->getOps();
	PCB* temp_pcb = new PCB;
	for(int i = 0; i < operations.size(); i++){
		log(runOperation(operations[i],thread_id,tp,process->getPCB()));
	}

}

//runs the current code and returns relevant time and operational data in a formatted string
std::string MetaData::runOperation(Code* code, pthread_t thread_id, std::chrono::high_resolution_clock::time_point tp, PCB* pcb){
	std::string ret_string = "";
	int* wait_time = new int;
	int cycle_time;
	
	Device temp_device;
	int temp_device_id;
	std::string temp_device_string;
	std::string description = code->getDescription();
	switch(code->getOperation()){
		//If the operations is System,  end or begin simulation as indicated
		//Make sure that the PCB is in appropriate states for beginning or ending the 
		//simulation and if it is not, throw an error and terminate.
		case 'S':
			if(code->getDescription() == "begin"){
				ret_string = ret_string + getTimeStamp(tp) + " - Simulator program starting\n";
			}
			else if(code->getDescription() == "finish"){
				ret_string = getTimeStamp(tp) + " - Simulator program ending\n";
			}
			return ret_string;
			break;
		//If the operation is Application, end or start the application as indicated
		//Increment the PCB ID as appropriate
		//If the PCB is not in an appropriate state for starting or ending an application
		//throw an error and terminate
		case 'A':
			if(code->getDescription() == "begin"){
				if((pcb->getState() != START) && (pcb->getState() != READY)){
					std::string error = "Tried to begin process with PCB not in START state, check metadat code\n";
					throw std::logic_error(error);
				}
				pcb->setState(READY);
				pcb->setID(pcb->getID() + 1);
				ret_string = getTimeStamp(tp) + " - OS: preparing process " + std::to_string(pcb->getID()) + "\n";
			}
			else if(code->getDescription() == "finish"){
				if(false){
					std::string error = "Tried to finish process in PCB not in READY state, check metadata code\n";
					throw std::logic_error(error);
				}
				pcb->setState(READY);
				ret_string = getTimeStamp(tp) + " - OS: removing process " + std::to_string(pcb->getID()) + "\n";
			}
			return ret_string;
			break;
		//If the operation is Processing, run the processor for the appropriate number
		//of cycles 
		case 'P':
			if(code->getDescription() == "run"){
				pcb->setState(RUNNING);
				ret_string = getTimeStamp(tp) + " - Process " + std::to_string(pcb->getID()) + ": start processing action\n";
				wait(code->getCycles() * config.getProcessorCycleTime());
				ret_string = ret_string + getTimeStamp(tp) + " - Process " + std::to_string(pcb->getID()) + ": end processing action\n";
				pcb->setState(READY);
			}
			return ret_string;
			break;
		//If the operation is input, process input using pthreads
		case 'I':
			//select the device and set some variables for printing and device management
			//description = code->getDescription();
			if(description == "hard drive"){
				temp_device_string = "HDD";
				temp_device = HDD;
				temp_device_id = resources->hdd_deque.front();
			}
			else if(description == "monitor"){
				temp_device_string = "MON";
				temp_device = MONITOR;
				temp_device_id = resources->monitor_deque.front();
			}
			else if(description == "keyboard"){
				temp_device_string = "KB";
				temp_device = KEYBOARD;
				temp_device_id = resources->keyboard_deque.front();
			}
			else if(description == "scanner"){
				temp_device_string = "SCAN";
				temp_device = SCANNER;
				temp_device_id = resources->scanner_deque.front();
			}
			else if(description == "printer"){
				temp_device_string = "PRIN";
				temp_device = PRINTER;
				temp_device_id = resources->printer_deque.front();
			}
			else if (description == "projector"){
				temp_device_string = "PROJ";
				temp_device = PROJECTOR;
				temp_device_id = resources->projector_deque.front();
			}
			else if(description == "mouse"){
				temp_device_string = "MOUSE";
				temp_device = MOUSE;
				temp_device_id = resources->mouse_deque.front();
			}

			//manage the device deque
			resources->manageDevice(temp_device);

			ret_string = getTimeStamp(tp) + " - Process " + std::to_string(pcb->getID()) + ": start " + code->getDescription() + " input on " + temp_device_string + " " + std::to_string(temp_device_id) + "\n";

			//sem_wait for the appropriate device and lock it
			switch(temp_device){
				case HDD:
					sem_wait(&hdd_sem);
					pthread_mutex_lock(&hdd_lock);
					break;
				case MONITOR:
					sem_wait(&monitor_sem);
					pthread_mutex_lock(&monitor_lock);
					break;
				case KEYBOARD:
					sem_wait(&keyboard_sem);
					pthread_mutex_lock(&keyboard_lock);
					break;
				case SCANNER:
					sem_wait(&scanner_sem);
					pthread_mutex_lock(&scanner_lock);
					break;
				case PRINTER:
					sem_wait(&printer_sem);
					pthread_mutex_lock(&printer_lock);
					break;
				case PROJECTOR:
					sem_wait(&projector_sem);
					pthread_mutex_lock(&projector_lock);
					break;
				case MOUSE:
					sem_wait(&mouse_sem);
					pthread_mutex_lock(&mouse_lock);
					break;
			}
			pcb->setState(WAITING);
			*wait_time = getCodeTime(*code);
			pthread_create(&thread_id,NULL,threadTimer,(void*)wait_time);
			pthread_join(thread_id,NULL);
			pcb->setState(READY);
			//sem_post for the appropriate device and unlock it
			switch(temp_device){
				case HDD:
					sem_post(&hdd_sem);
					pthread_mutex_unlock(&hdd_lock);
					break;
				case MONITOR:
					sem_post(&monitor_sem);
					pthread_mutex_unlock(&monitor_lock);
					break;
				case KEYBOARD:
					sem_post(&keyboard_sem);
					pthread_mutex_unlock(&keyboard_lock);
					break;
				case SCANNER:
					sem_post(&scanner_sem);
					pthread_mutex_unlock(&scanner_lock);
					break;
				case PRINTER:
					sem_post(&printer_sem);
					pthread_mutex_unlock(&printer_lock);
					break;
				case PROJECTOR:
					sem_post(&projector_sem);
					pthread_mutex_unlock(&projector_lock);
					break;
				case MOUSE:
					sem_post(&mouse_sem);
					pthread_mutex_unlock(&mouse_lock);
					break;
			}	
			ret_string = ret_string + getTimeStamp(tp) + " - Process " + std::to_string(pcb->getID()) + ": end " + code->getDescription() + " input\n";
			return ret_string;
			break;
		//If the operation is output, process output using pthreads
		case 'O':
			description = code->getDescription();
			if(description == "hard drive"){
					temp_device_string = "HDD";
					temp_device = HDD;
					temp_device_id = resources->hdd_deque.front();
			}
			else if(description == "monitor"){
				temp_device_string = "MON";
				temp_device = MONITOR;
				temp_device_id = resources->monitor_deque.front();
			}
			else if(description == "keyboard"){
				temp_device_string = "KB";
				temp_device = KEYBOARD;
				temp_device_id = resources->keyboard_deque.front();
			}
			else if(description == "scanner"){
				temp_device_string = "SCAN";
				temp_device = SCANNER;
				temp_device_id = resources->scanner_deque.front();
			}
			else if(description == "printer"){
				temp_device_string = "PRIN";
				temp_device = PRINTER;
				temp_device_id = resources->printer_deque.front();
			}
			else if (description == "projector"){
				temp_device_string = "PROJ";
				temp_device = PROJECTOR;
				temp_device_id = resources->projector_deque.front();
			}
			else if(description == "mouse"){
				temp_device_string = "MOUSE";
				temp_device = MOUSE;
				temp_device_id = resources->mouse_deque.front();
			}

			//manage the device deque
			resources->manageDevice(temp_device);

			ret_string = getTimeStamp(tp) + " - Process " + std::to_string(pcb->getID()) + ": start " + code->getDescription() + " output on " + temp_device_string + " " + std::to_string(temp_device_id) + "\n";
			
			//sem_wait for the appropriate device and lock it
			switch(temp_device){
				case HDD:
					sem_wait(&hdd_sem);
					pthread_mutex_lock(&hdd_lock);
					break;
				case MONITOR:
					sem_wait(&monitor_sem);
					pthread_mutex_lock(&monitor_lock);
					break;
				case KEYBOARD:
					sem_wait(&keyboard_sem);
					pthread_mutex_lock(&keyboard_lock);
					break;
				case SCANNER:
					sem_wait(&scanner_sem);
					pthread_mutex_lock(&scanner_lock);
					break;
				case PRINTER:
					sem_wait(&printer_sem);
					pthread_mutex_lock(&printer_lock);
					break;
				case PROJECTOR:
					sem_wait(&projector_sem);
					pthread_mutex_lock(&projector_lock);
					break;
				case MOUSE:
					sem_wait(&mouse_sem);
					pthread_mutex_lock(&mouse_lock);
					break;
			}
			pcb->setState(WAITING);
			*wait_time = getCodeTime(*code);
			pthread_create(&thread_id,NULL,threadTimer,(void*)wait_time);
			pthread_join(thread_id,NULL);
			pcb->setState(READY);
			//sem_post the appropriate device and unlock it
			switch(temp_device){
				case HDD:
					sem_post(&hdd_sem);
					pthread_mutex_unlock(&hdd_lock);
					break;
				case MONITOR:
					sem_post(&monitor_sem);
					pthread_mutex_unlock(&monitor_lock);
					break;
				case KEYBOARD:
					sem_post(&keyboard_sem);
					pthread_mutex_unlock(&keyboard_lock);
					break;
				case SCANNER:
					sem_post(&scanner_sem);
					pthread_mutex_unlock(&scanner_lock);
					break;
				case PRINTER:
					sem_post(&printer_sem);
					pthread_mutex_unlock(&printer_lock);
					break;
				case PROJECTOR:
					sem_post(&projector_sem);
					pthread_mutex_unlock(&projector_lock);
					break;
				case MOUSE:
					sem_post(&mouse_sem);
					pthread_mutex_unlock(&mouse_lock);
					break;
			}	
			ret_string = ret_string + getTimeStamp(tp) + " - Process " + std::to_string(pcb->getID()) + ": end " + code->getDescription() + " output\n";
			return ret_string;
			break;
		//If the operation is in memory, perform the relevant operation
		case 'M':
			if(code->getDescription() == "block"){
				pcb->setState(RUNNING);
				ret_string = getTimeStamp(tp) + " - Process " + std::to_string(pcb->getID()) + ": start memory blocking\n";
				wait(code->getCycles() * config.getMemoryCycleTime());
				pcb->setState(READY);
				ret_string = ret_string + getTimeStamp(tp) + " - Process " + std::to_string(pcb->getID()) + ": end memory blocking\n";
			}
			if(code->getDescription() == "allocate"){
				pcb->setState(RUNNING);
				ret_string = getTimeStamp(tp) + " - Process " + std::to_string(pcb->getID()) + ": allocating memory\n";
				wait(code->getCycles() * config.getMemoryCycleTime());
				pcb->setState(READY);
				ret_string = ret_string + getTimeStamp(tp) + " - Process " + std::to_string(pcb->getID()) + ": memory allocated at " + allocateMemory() + "\n";
			}
			return ret_string;
			break;
	}
}

//Randomly allocate a valid memory address for the program and return the value as a printable
//string
std::string MetaData::allocateMemory(){
	std::stringstream sstream;
	unsigned int memory_location = next_memory_block;
	next_memory_block += config.getBlockSize();
	if(next_memory_block >= config.getSystemMemory()){
		next_memory_block = 0;
	}
	sstream << "0x" << std::hex << memory_location;
	std::string ret_string(sstream.str());
	return ret_string;
}
		
//log all metadata to the appropriate channels based on what is set in config
void MetaData::log(){
	std::ofstream log_file;
	if(config.logToFile()){
		log_file.open(config.getLogFilename().c_str(),std::ofstream::app);
		log_file << *this;
		log_file.close();
	}
	if(config.logToConsole()){
		std::cout << *this;
	}
}

//log string to the appropriate channels based on what is set in config
void MetaData::log(std::string s){
	std::ofstream log_file;
	if(config.logToFile()){
		log_file.open(config.getLogFilename().c_str(),std::ofstream::app);
		log_file << s;
		log_file.close();
	}
	if(config.logToConsole()){
		std::cout << s;
	}
}

//Operator overload for << operator on MetaData class
std::ostream& operator<<(std::ostream& os, MetaData md){
	os << "Meta-Data Metrics" << std::endl;
	std::vector<Code*> cds = md.getCodes();
	for(int i = 0; i < cds.size(); i++){
		if(cds[i]->logCode()){
			os << *cds[i] << " - " << md.getCodeTime(*cds[i]) << " ms" << std::endl;
		}
	}
	return os;
}

//Parses metadata file and returns a vector of code objects
std::vector<Code*> parseMetadataFile(std::string metadata_filename){
	std::vector<Code*> codes;

	std::string temp_string;
	std::string error;
	std::string file_extension;
	std::ifstream metadata_file;
	
	//get the file extension and check if it is mdf, else throw an error
	file_extension = metadata_filename.substr(metadata_filename.find_last_of('.') + 1);
	if(file_extension != "mdf"){
		error = "Invalid file extension for metadata file: " + file_extension + "\n";
		throw std::logic_error(error);
	}
	//open the file and check if it is good, if not throw an error
	metadata_file.open(metadata_filename.c_str());
	if(!metadata_file.good()){
		error = "Invalid filename for metadata file: " + metadata_filename + "\n";
		throw std::logic_error(error);
	}
	//check if the file is empty, if it is throw an error
	if(metadata_file.peek() == std::ifstream::traits_type::eof()){
		throw std::logic_error("Metadata file is valid file, but empty\n");
	}

	//skip the first "Start Program Meta-data Code:" line
	std::getline(metadata_file,temp_string);

	//While not at the end of file, get strings to pass to the Code parameterized constructor to create individual codes
	//push these codes into the codes vector
	while(!metadata_file.eof()){
		//skip white-space
		while((metadata_file.peek() == ' ') || (metadata_file.peek() == '\n')){
			metadata_file.get();
		}
		std::getline(metadata_file,temp_string,';');
		if(temp_string == "End Program Meta-Data Code."){
			return codes;
		}
		Code* temp_code = new Code(temp_string);
		codes.push_back(temp_code);
	}
	return codes;
}
