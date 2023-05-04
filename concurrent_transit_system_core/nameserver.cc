#include "nameserver.h"
#include "global.h"
#include "printer.h"
#include "trainstop.h"

NameServer::NameServer( Printer & prt, unsigned int numStops ):
    P{prt}, numStops{numStops} {
        trainStops = new TrainStop*[numStops];
}

void NameServer::registerStop( unsigned int trainStopId ){
    P.print(Printer::Kind::NameServer, static_cast<char>(State::Register), trainStopId);
    trainStops[trainStopId] = (TrainStop*) &uThisTask();
}

TrainStop * NameServer::getStop( unsigned int studentId, unsigned int trainStopId ){
    P.print(Printer::Kind::NameServer, static_cast<char>(State::Identity), studentId, trainStopId);
    return trainStops[trainStopId];
}

TrainStop ** NameServer::getStopList(){
    P.print(Printer::Kind::NameServer, static_cast<char>(State::List));
    return trainStops;
}

TrainStop ** NameServer::getStopList( unsigned int trainId ){
    P.print(Printer::Kind::NameServer, static_cast<char>(State::List), trainId);
    return trainStops;
}

unsigned int NameServer::getNumStops(){
    return numStops;
}

void NameServer::main(){
    P.print(Printer::Kind::NameServer, static_cast<char>(State::Start));

    // register stops first
    for(unsigned int i = 0; i < numStops; i++){
        _Accept(registerStop){}
    }

    // register stops first and after registering for all the stops
    for(;;){
        _Accept(~NameServer){
            break;
        } or _Accept(getStop || getStopList || getNumStops){}
    }
    P.print(Printer::Kind::NameServer, static_cast<char>(State::Finished));
}

NameServer::~NameServer()
{
    delete []trainStops;
}