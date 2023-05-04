#pragma once
#include "global.h"
#include <map>
#include <uPRNG.h>

class WATCard;

struct PassengerInfo;

_Task Train {
    public:
        _Event Ejected {};						// Exception raised at student without ticket
        enum Direction { Clockwise, CounterClockwise };
        Train( Printer & prt, NameServer & nameServer, unsigned int id, unsigned int maxNumStudents, unsigned int numStops );
        ~Train();
        _Nomutex unsigned int getId() const;
        TrainStop * embark( unsigned int studentId, unsigned int destStop, WATCard& card ); // Student waits until train reaches destination stop.
        void scanPassengers(); // called by conductor
    private:
        Printer & P;
        NameServer & nameServer;
        unsigned int id;
        unsigned int maxNumStudents;
        unsigned int numStops;

        TrainStop** trainstops;
        uCondition passengers;

        unsigned int curStop;
        int nextStopAddr;
        std::map<int, PassengerInfo> studentIdToPassengerInfo;

        // curStudent Information
        unsigned int studentId;
        unsigned int destStop;

        enum State {Start = 'S', Arrive = 'A', Embark = 'E', Finished = 'F', ClockwiseChar = '<', CounterClockwiseChar = '>', ConductorEject = 'e'};

        void main();
};

// Local Variables: //
// mode: c++ //
// tab-width: 4 //
// compile-command: "make" //
// End: //
