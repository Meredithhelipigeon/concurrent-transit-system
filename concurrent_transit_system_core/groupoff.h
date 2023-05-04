#pragma once

#include "watcard.h"

_Monitor Printer;

_Task Groupoff {
    Printer & P;
    unsigned int numStudents;
    unsigned int maxTripCost;
    unsigned int groupoffDelay;
    // keep track of all the future values
    WATCard::FWATCard * watCards;
    // keep track of all the real cards
    WATCard ** cards;
    int* watCardsIndex;

    unsigned int assignedNum = 0;

    enum State {Start = 'S', Deposit = 'D', Finished = 'F'};

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
