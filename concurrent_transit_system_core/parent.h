#pragma once

_Monitor Printer;										// forward declarations
_Monitor Bank;

_Task Parent {
    Printer & P;
    Bank & bank;
    unsigned int numStudents;
    unsigned int parentDelay;
    unsigned int maxTripCost;

     enum State {Start = 'S', Deposit = 'D', Finished = 'F'};
    void main();
public:
    Parent( Printer & prt, Bank & bank, unsigned int numStudents, unsigned int parentalDelay, unsigned int maxTripCost );
    ~Parent();
}; // Parent

// Local Variables: //
// mode: c++ //
// tab-width: 4 //
// compile-command: "make" //
// End: //
