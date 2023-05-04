#pragma once

_Monitor Printer {
  public:
	enum Kind { Parent, Groupoff, WATCardOffice, NameServer, Timer, Train, Conductor, TrainStop, Student, WATCardOfficeCourier };

  private:
  	class PImpl;
  	PImpl * pimpl;

  public:
	Printer( unsigned int numStudents, unsigned int numTrains, unsigned int numStops, unsigned int numCouriers );
	~Printer();
	void print( Kind kind, char state );
	void print( Kind kind, char state, unsigned int value1 );
	void print( Kind kind, char state, unsigned int value1, unsigned int value2 );
	void print( Kind kind, unsigned int lid, char state );
	void print( Kind kind, unsigned int lid, char state, unsigned int value1 );
	void print( Kind kind, unsigned int lid, char state, unsigned int value1, unsigned int value2 );
	void print( Kind kind, unsigned int lid, char state, unsigned int oid, unsigned int value1, unsigned int value2 );
	void print( Kind kind, unsigned int lid, char state, unsigned int value1, char c );
	void print( Kind kind, unsigned int lid, char state, unsigned int value1, unsigned int value2, char c );
}; // Printer

// Local Variables: //
// mode: c++ //
// tab-width: 4 //
// compile-command: "make" //
// End: //
