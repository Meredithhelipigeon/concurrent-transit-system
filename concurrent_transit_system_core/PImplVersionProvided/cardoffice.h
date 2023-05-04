#pragma once

#include "watcard.h"
#include <list>

_Monitor Printer;										// forward declarations
_Monitor Bank;
struct Job;

_Task WATCardOffice {
	class PImpl;
	PImpl * pimpl;

	void main();
  public:
	_Event Lost {};										// lost WATCard
	WATCardOffice( Printer & prt, Bank & bank, unsigned int numCouriers );
	~WATCardOffice();
	WATCard::FWATCard create( unsigned int sid, unsigned int amount ); // called by student
	WATCard::FWATCard transfer( unsigned int sid, unsigned int amount, WATCard * card ); // called by student
	Job * requestWork();								// called by courier to request/return work
}; // WATCardOffice

// Local Variables: //
// mode: c++ //
// tab-width: 4 //
// compile-command: "make" //
// End: //
