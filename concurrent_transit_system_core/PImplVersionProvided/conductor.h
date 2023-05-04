#pragma once
#include "global.h"

_Task Train;

_Task Conductor {
	class PImpl;
	PImpl * pimpl;

	void main();
  public:
  	Conductor( Printer & prt, unsigned int id, Train * train, unsigned int delay );
  	~Conductor();
};


// Local Variables: //
// mode: c++ //
// tab-width: 4 //
// compile-command: "make" //
// End: //
