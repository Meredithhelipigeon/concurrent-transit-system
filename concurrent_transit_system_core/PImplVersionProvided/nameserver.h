#pragma once

_Monitor Printer;										// forward declarations
_Task TrainStop;

_Task NameServer {
	class PImpl;
	PImpl * pimpl;
	
	void main();
  public:
	NameServer( Printer & prt, unsigned int numStops );
	~NameServer();
	void registerStop( unsigned int trainStopId );
	TrainStop * getStop( unsigned int studentId, unsigned int trainStopId );
	TrainStop ** getStopList();  // called by Timer
	TrainStop ** getStopList( unsigned int trainId );	// called by Train
	unsigned int getNumStops();
};

// Local Variables: //
// mode: c++ //
// tab-width: 4 //
// compile-command: "make" //
// End: //
