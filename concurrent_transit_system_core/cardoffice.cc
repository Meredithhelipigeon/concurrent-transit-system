#include "cardoffice.h"
#include "bank.h"
#include "printer.h"

void WATCardOffice::Courier::main() {
    // WATCardOffice::Courier starts
    P.print(Printer::Kind::WATCardOfficeCourier, id, 'S');
    // start looping to pick up job
    Job *curJob = nullptr;
    for ( ;; ) {
        // get next job
        curJob = cardOffice.requestWork();
        // terminate if job is nullptr
        if (curJob == nullptr) break;
        // otherwise, we process the job
        P.print(Printer::Kind::WATCardOfficeCourier, id, 't', curJob->sid, curJob->amount);
        // obtain money from bank
        bank.withdraw(curJob->sid, curJob->amount);
        // update student watcard
        curJob->watCard->deposit(curJob->amount);
        P.print(Printer::Kind::WATCardOfficeCourier, id, 'T', curJob->sid, curJob->amount);
        // loss card in chance of 1/6
        if (prng(5) == 0) {
            P.print(Printer::Kind::WATCardOfficeCourier, id, 'L', curJob->sid);
            // delete current watcard
            delete curJob->watCard;
            curJob->result.delivery(new WATCardOffice::Lost());
        } else {
            // insert into future watcard
            curJob->result.delivery(curJob->watCard);
        }
        // job done, delete it
        delete curJob;
    }
    // WATCardOffice::Courier ends
    P.print(Printer::Kind::WATCardOfficeCourier, id, 'F');
}

void WATCardOffice::main() {
    // WATCardOffice starts
    P.print(Printer::Kind::WATCardOffice, 'S');
    // start looping
    for ( ;; ) {
        _Accept(~WATCardOffice) {
            break;
        } or _Accept(create || transfer) {
            // wake up a waiting courier to proceed as job has been added
            block.signalBlock();
        } or _Accept(requestWork) {
            // do nothing
        }
    }
    // WATCardOffice done, need to wake up all waiting couriers
    for (unsigned int i = 0; i < numCouriers; i++) {
        // pass nullptr to terminate courier
        jobList.push_back(nullptr);
        // wake up waiting courier and wait for it to finish
        block.signalBlock();
    }
    // WATCardOffice finishes
    P.print(Printer::Kind::WATCardOffice, 'F');
}

// constructor, allocate memory for courierList
WATCardOffice::WATCardOffice( Printer & prt, Bank & bank, unsigned int numCouriers ) : 
    P(prt), bank(bank), numCouriers(numCouriers) {
    // fixed number of couriers
    courierList = new Courier*[numCouriers];
    // allocate memory for couriers
    for (unsigned int i = 0; i< numCouriers; i++) {
        courierList[i] = new Courier(P, bank, *this, i);
    }
}

// destructor, de-allocate memory for courierList
WATCardOffice::~WATCardOffice() {
    for (unsigned int i = 0; i < numCouriers; i++) {
        if (courierList[i] != nullptr) delete courierList[i];
    }
    delete [] courierList;
}

// create watcard and corresponding job (async)
// return Future WatCard
// note: job will be picked up by courier periodically
WATCard::FWATCard WATCardOffice::create( unsigned int sid, unsigned int amount ) {
    // create watcard and pass to job
    WATCard *watcard = new WATCard();
    // new job is created and added to WATCardOffice job list
    Job *job = new Job(sid, amount, watcard);
    jobList.push_back(job);
    P.print( Printer::Kind::WATCardOffice, 'C', sid, amount );
    return job->result;
}

// transfer amount of money into the card (async)
// return Future WatCard
// note: job will be picked up by courier periodically
WATCard::FWATCard WATCardOffice::transfer( unsigned int sid, unsigned int amount, WATCard * card ) {
    // create new job and added to WATCardOffice job list
    Job *job = new Job(sid, amount, card);
    jobList.push_back(job);
    P.print( Printer::Kind::WATCardOffice, 'T', sid, amount );
    return job->result;
}

// get a job from jobList if ther exists any in FIFO order and remove it from the jobList
WATCardOffice::Job * WATCardOffice::requestWork() {
    // block on condition if no jobs available
    while (jobList.empty()) block.wait();
    // next job is ready
    Job *ret = jobList.front();
    jobList.pop_front();
    P.print(Printer::Kind::WATCardOffice, 'W');
    return ret;
}
