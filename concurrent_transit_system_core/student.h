#pragma once
#include "global.h"
#include "watcard.h"
#include "train.h"

_Task Student {
    unsigned int id;
    unsigned int trainTrip;
    unsigned int maxStudentDelay;
    unsigned int numStops;
    unsigned int stopCost;

    Printer & P;
    NameServer & nameServer;
    WATCardOffice & cardOffice;
    Groupoff & groupoff;

    Future_ISM<WATCard *> FWATCard;
    Future_ISM<WATCard *> GiftCard;
    int maxCostOfTrip;

    // current trip information
    unsigned int startStop;
    unsigned int endStop;
    Train::Direction direction;
    unsigned int tripStops;
    bool ifGiftUsed = false;

    enum State {Start = 'S', Trip = 'T', GiftTicket = 'G', WatCardTicket = 'B', FreeRide = 'f', Ejected = 'e',
    Wait = 'W', Embark = 'E', Disembark = 'D', LostCrad = 'L', Finished = 'F', Clockwise = '<', CounterClockwise = '>'};

    void main();
    void checkDirection();
public:
    Student( Printer & prt, NameServer & nameServer, WATCardOffice & cardOffice, Groupoff & groupoff, 
        unsigned int id, unsigned int numStops, unsigned int stopCost, unsigned int maxStudentDelay, unsigned int maxStudentTrips );
    ~Student();
};

// Local Variables: //
// mode: c++ //
// tab-width: 4 //
// compile-command: "make" //
// End: //
