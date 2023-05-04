#pragma once

#include "global.h"
#include "nameserver.h"
#include "trainstop.h"

_Task Timer {
    Printer & P;
    NameServer & nameServer;
    unsigned int timeDelay;
    TrainStop ** trainstops;
    unsigned int numStops;

    enum State {Start = 'S', Tick = 't', Finished = 'F'};

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
