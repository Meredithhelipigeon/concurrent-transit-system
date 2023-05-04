#pragma once

#include "global.h"

_Task Timer {
	class PImpl;
	PImpl * pimpl;

    void main();
  public:
    Timer( Printer & prt, NameServer & nameServer, unsigned int timerDelay );
    ~Timer();
};

// Local Variables: //
// mode: c++ //
// tab-width: 4 //
// compile-command: "make" //
// End: //
