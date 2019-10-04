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
	if((temp_string != "begin") &&(temp_string != "finish") && (temp_string != "harddrive") && (temp_string != "keyboard") && (temp_string != "mouse") && (temp_string != "monitor") && (temp_string != "run") && (temp_string != "allocate") && (temp_string != "printer") && (temp_string != "block")){
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
	codes =cds;
}

//Parameterized Constructor for MetaData class
MetaData::MetaData(Config cfg){ 
	config = cfg;
	codes = parseMetadataFile(config.getMetadataFilename());
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

//log to the appropriate channels based on what is set in config
void MetaData::log(){
	std::ofstream log_file;
	std::cout << std::endl;
	std::cout << config.getLogFilename() << std::endl;
	if(config.logToFile()){
		log_file.open(config.getLogFilename().c_str(),std::ofstream::app);
		log_file << *this;
		log_file.close();
	}
	if(config.logToConsole()){
		std::cout << *this;
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
