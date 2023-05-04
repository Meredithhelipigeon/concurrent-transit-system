#pragma once
#include "global.h"

_Task Train;

_Task Conductor {
    Printer & P;
    unsigned int id;
    Train * train;
    unsigned int delay;
    enum State {Start = 'S', Check = 'c', Eject = 'e', Finished = 'F'};
    
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
