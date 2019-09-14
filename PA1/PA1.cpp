#include "config.h"
#include "metadata.h"

int main(int argc, char** argv){
	//if there are no command line arguments, no file was given
	if(argc < 2){
		std::cout << "No config file name given." << std::endl;
		return 0;
	}
	//if there are too many command line arguments show error
	if(argc > 2){
		std::cout << "Too many command line arguments, expecting only config filename" << std::endl;
		return 0;
	}
	//intialize config file name with command line parameter
	std::string config_filename = argv[1];
	//create a config object using the parameterized constructor
	Config* cfg = new Config(config_filename);
	//log the config settings
	cfg->log();
	//create a metadata object using the parameterized constructor
	MetaData* metadata = new MetaData(*cfg);
	//log the metadta code
	metadata->log();
}
