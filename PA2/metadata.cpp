//--------------------------------------------------------------------
// Jonathon Hewitt
// metadata.cpp
// Implementation of the Code and MetaData classes along with 
// relevant functions
//--------------------------------------------------------------------

#include "metadata.h"

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
	pcb.setID(0);
	pcb.setState(DEFAULT);
}

//Parameterized Constructor for MetaData class
MetaData::MetaData(Config cfg){ 
	config = cfg;
	codes = parseMetadataFile(config.getMetadataFilename());
	pcb.setID(0);
	pcb.setState(DEFAULT);
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
	std::string log_string;
	pthread_t thread_id;
	std::chrono::high_resolution_clock::time_point tp;
	tp = std::chrono::high_resolution_clock::now();
	for(int i = 0; i < codes.size(); i++){
		log(runProcess(codes[i],thread_id,tp));
	}
}

//runs the current code and returns relevant time and operational data in a formatted string
std::string MetaData::runProcess(Code* code, pthread_t thread_id, std::chrono::high_resolution_clock::time_point tp){
	std::string ret_string = "";
	int* wait_time = new int;
	int cycle_time;
	switch(code->getOperation()){
		//If the operations is System,  end or begin simulation as indicated
		//Make sure that the PCB is in appropriate states for beginning or ending the 
		//simulation and if it is not, throw an error and terminate.
		case 'S':
			if(code->getDescription() == "begin"){
				if(pcb.getState() != DEFAULT){
					std::string error = "Tried to begin process with PCB in not intial state, check metadata code\n";
					throw std::logic_error(error);
				}
				pcb.setState(START);
				ret_string = ret_string + getTimeStamp(tp) + " - Simulator program starting\n";
			}
			else if(code->getDescription() == "finish"){
				if(pcb.getState() != READY){
					std::string error = "Tried to finish process with PCB not in ready state, check metadata code\n";
					throw std::logic_error(error);
				}
				pcb.setState(EXIT);
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
				if((pcb.getState() != START) && (pcb.getState() != READY)){
					std::string error = "Tried to begin process with PCB not in START state, check metadat code\n";
					throw std::logic_error(error);
				}
				pcb.setState(READY);
				pcb.setID(pcb.getID() + 1);
				ret_string = getTimeStamp(tp) + " - OS: preparing process " + std::to_string(pcb.getID()) + "\n";
			}
			else if(code->getDescription() == "finish"){
				if(pcb.getState() != READY){
					std::string error = "Tried to finish process in PCB not in READY state, check metadata code\n";
					throw std::logic_error(error);
				}
				pcb.setState(READY);
				ret_string = getTimeStamp(tp) + " - OS: removing process " + std::to_string(pcb.getID()) + "\n";
			}
			return ret_string;
			break;
		//If the operation is Processing, run the processor for the appropriate number
		//of cycles 
		case 'P':
			if(code->getDescription() == "run"){
				pcb.setState(RUNNING);
				ret_string = getTimeStamp(tp) + " - Process " + std::to_string(pcb.getID()) + ": start processing action\n";
				wait(code->getCycles() * config.getProcessorCycleTime());
				ret_string = ret_string + getTimeStamp(tp) + " - Process " + std::to_string(pcb.getID()) + ": end processing action\n";
				pcb.setState(READY);
			}
			return ret_string;
			break;
		//If the operation is input, process input using pthreads
		case 'I':
			ret_string = getTimeStamp(tp) + " - Process " + std::to_string(pcb.getID()) + ": start " + code->getDescription() + " input\n";
			pcb.setState(WAITING);
			*wait_time = getCodeTime(*code);
			pthread_create(&thread_id,NULL,threadTimer,(void*)wait_time);
			pthread_join(thread_id,NULL);
			pcb.setState(READY);
			ret_string = ret_string + getTimeStamp(tp) + " - Process " + std::to_string(pcb.getID()) + ": end " + code->getDescription() + " input\n";
			return ret_string;
			break;
		//If the operation is output, process output using pthreads
		case 'O':
			ret_string = getTimeStamp(tp) + " - Process " + std::to_string(pcb.getID()) + ": start " + code->getDescription() + " output\n";
			pcb.setState(WAITING);
			*wait_time = getCodeTime(*code);
			pthread_create(&thread_id,NULL,threadTimer,(void*)wait_time);
			pthread_join(thread_id,NULL);
			pcb.setState(READY);
			ret_string = ret_string + getTimeStamp(tp) + " - Process " + std::to_string(pcb.getID()) + ": end " + code->getDescription() + " output\n";
			return ret_string;
			break;
		//If the operation is in memory, perform the relevant operation
		case 'M':
			if(code->getDescription() == "block"){
				pcb.setState(RUNNING);
				ret_string = getTimeStamp(tp) + " - Process " + std::to_string(pcb.getID()) + ": start memory blocking\n";
				wait(code->getCycles() * config.getMemoryCycleTime());
				pcb.setState(READY);
				ret_string = ret_string + getTimeStamp(tp) + " - Process " + std::to_string(pcb.getID()) + ": end memory blocking\n";
			}
			if(code->getDescription() == "allocate"){
				pcb.setState(RUNNING);
				ret_string = getTimeStamp(tp) + " - Process " + std::to_string(pcb.getID()) + ": allocating memory\n";
				wait(code->getCycles() * config.getMemoryCycleTime());
				pcb.setState(READY);
				ret_string = ret_string + getTimeStamp(tp) + " - Process " + std::to_string(pcb.getID()) + ": memory allocated at " + allocateMemory() + "\n";
			}
			return ret_string;
			break;
	}
}

//Randomly allocate a valid memory address for the program and return the value as a printable
//string
std::string MetaData::allocateMemory(){
	int memory_location;
	srand(time(NULL));
	if(config.getMemorySize() == "kbytes"){
		memory_location = rand() % config.getSystemMemory() + 1;
	}
	if(config.getMemorySize() == "Mbytes"){
		memory_location = rand() % (config.getSystemMemory()*1000) + 1;
	}
 	if(config.getMemorySize() == "Gbtes"){
		memory_location = rand() % (config.getSystemMemory() * 1000000) + 1;
	}
	std::stringstream sstream;
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
