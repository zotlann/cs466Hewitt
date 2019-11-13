#ifndef CODE_H
#define CODE_H

#include <iostream>
#include <string>


class Code{
	public:
		//Constructors
		Code();
		Code(std::string);

		//Getters and setters
		char getOperation();
		void setOperation(char);

		std::string getDescription();
		void setDescription(std::string);

		int getCycles();
		void setCycles(int);
		
		//whether or not a code should be output and logged
		bool logCode();

		//format the code in a nice print friendly way
		friend std::ostream& operator<<(std::ostream& os,Code code);
	private:
		char operation;
		std::string description;
		int cycles;
};
#endif //CODE_H
