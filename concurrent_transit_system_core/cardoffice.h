#pragma once

#include "watcard.h"
#include "uPRNG.h"
#include "uFuture.h"
#include <list>

_Monitor Printer;										// forward declarations
_Monitor Bank;
struct Job;

/* WATCardOffice */
_Task WATCardOffice {
private:
    /* Job */
    struct Job { // marshalled arguments and return future
        unsigned int sid;         // student id
        unsigned int amount;      // fare cost
        WATCard *watCard;         // student watCard
        WATCard::FWATCard result; // return future
        Job( unsigned int sid, unsigned int amount, WATCard *watCard ) : 
            sid(sid), amount(amount), watCard(watCard) {}
    };

    /* Courier */
    _Task Courier {                                     // communicates with bank
      private:
        Printer             &P;
        Bank                &bank;
        WATCardOffice       &cardOffice;
        unsigned int        id;                         // courier id
        void main();
      public:
        Courier( Printer & prt, Bank & bank, WATCardOffice & cardOffice, unsigned int id ) :
            P(prt), bank(bank), cardOffice(cardOffice), id(id) {}
    }; // Courier

    void main();
    Printer                 &P;                         // printer
    Bank                    &bank;                      // back for withdraw money
    unsigned int            numCouriers;                // number of couriers from config file
    std::list<Job *>        jobList;                    // list of jobs
    Courier**               courierList;                // list of courierList, fixed size
    uCondition              block;                      // courier wait on this condition
public:
    _Event Lost {};                                     // lost WATCard
    WATCardOffice( Printer & prt, Bank & bank, unsigned int numCouriers );
    ~WATCardOffice();
    WATCard::FWATCard create( unsigned int sid, unsigned int amount ); // called by student
    WATCard::FWATCard transfer( unsigned int sid, unsigned int amount, WATCard * card ); // called by student
    Job * requestWork();                                // called by courier to request/return work
}; // WATCardOffice

// Local Variables: //
// mode: c++ //
// tab-width: 4 //
// compile-command: "make" //
// End: //
