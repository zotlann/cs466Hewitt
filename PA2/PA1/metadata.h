#ifndef METADATA_H
#define METADATA_H

#include "config.h"
#include <vector>
#include <sstream>
#include <algorithm>

class Code{
	public:
		Code();
		Code(std::string);

		char getOperation();
		void setOperation(char);

		std::string getDescription();
		void setDescription(std::string);

		int getCycles();
		void setCycles(int);
		
		bool logCode();

		friend std::ostream& operator<<(std::ostream& os,Code code);
	private:
		char operation;
		std::string description;
		int cycles;
};

class MetaData{
	public:
		MetaData();
		MetaData(Config);

		Config getConfig();
		void setConfig(Config);

		std::vector<Code*> getCodes();
		void setCodes(std::vector<Code*>);

		int getCodeTime(Code);

		void log();

		friend std::ostream& operator<<(std::ostream& os,MetaData metadata);
	private:
		Config config;
		std::vector<Code*> codes;
};

std::vector<Code*> parseMetadataFile(std::string);

#endif //METADATA_H
