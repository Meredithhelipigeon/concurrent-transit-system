#pragma once

#include "watcard.h"

_Monitor Printer;

_Task Groupoff {
	class PImpl;
	PImpl * pimpl;

	void main();
  public:
	Groupoff( Printer & prt, unsigned int numStudents, unsigned int maxTripCost, unsigned int groupoffDelay );
	~Groupoff();
	WATCard::FWATCard giftCard();
}; // Groupoff

// Local Variables: //
// mode: c++ //
// tab-width: 4 //
// compile-command: "make" //
// End: //
