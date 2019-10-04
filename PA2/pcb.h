//--------------------------------------------------------------------
// Jonathon Hewitt
// pcb.h
// Declaration of the PCB (process control block) class and relevant
// functions
//--------------------------------------------------------------------
#ifndef PCB_H
#define PCB_H

//enum for valid PCB states, default is only found when constructed and
//is used to indicate the PCB hasn't been altered yet
enum PCBState {DEFAULT,START,READY,RUNNING,WAITING,EXIT};

//PCB Class
class PCB{
	public:
		//Constructors
		PCB();
		PCB(int,PCBState);

		//Getters and setters
		int getID();
		PCBState getState();

		void setID(int);
		void setState(PCBState);

	private:
		int id;
		PCBState state;
};



#endif //PCB_H
