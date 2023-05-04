#include "train.h"
#include "trainstop.h"
#include <cmath>
#include "nameserver.h"
#include "watcard.h"
#include "printer.h"

struct PassengerInfo {
    bool ifEject;  
    WATCard & card;

    PassengerInfo() : ifEject(false), card(*new WATCard()) {}
    PassengerInfo(bool eject, WATCard& c) : ifEject(eject), card(c) {}
};

Train::Train( Printer & prt, NameServer & nameServer, unsigned int id, unsigned int maxNumStudents, unsigned int numStops ):
  P{ prt }, nameServer{ nameServer }, id{ id }, maxNumStudents { maxNumStudents }, numStops {numStops }{
      curStop = 0;
      nextStopAddr = 1;
      if (static_cast<Direction>(id)==CounterClockwise) {
        curStop = ceil(numStops/2.0);
        nextStopAddr = -1;
        P.print(Printer::Kind::Train, id ,static_cast<char>(State::Start), curStop, static_cast<char>(State::CounterClockwiseChar));
      } else {
        P.print(Printer::Kind::Train, id ,static_cast<char>(State::Start), curStop, static_cast<char>(State::ClockwiseChar));
      }
}

Train::~Train(){
    P.print(Printer::Kind::Train, id ,static_cast<char>(State::Finished));
}

_Nomutex unsigned int Train::getId() const{
    return id;
}

TrainStop * Train::embark( unsigned int studentId, unsigned int destStop, WATCard& card ){
    Train::studentId = studentId;
    Train::destStop = destStop;

    studentIdToPassengerInfo.insert(std::make_pair(studentId, PassengerInfo{false, card}));
    // if this student hasnt arrived the destination/been caught, he has to wait here even though he is waken up
    while (curStop!=destStop && !studentIdToPassengerInfo[studentId].ifEject){
        passengers.wait(studentId);
    }
    if (studentIdToPassengerInfo[studentId].ifEject){
        studentIdToPassengerInfo.erase(studentId);
        _Throw Ejected();
    } else {
        studentIdToPassengerInfo.erase(studentId);
    }

    return trainstops[destStop];
}

void Train::scanPassengers(){
    for (auto & [studentId, passengerInfo] : studentIdToPassengerInfo) {
        // check if the passengers have paid the ticket
        if (!passengerInfo.card.paidForTicket() && (!passengerInfo.ifEject)){
            P.print(Printer::Kind::Conductor, id, static_cast<char>(State::ConductorEject), studentId);
            passengerInfo.ifEject = true;
        }
    }
}

void Train::main(){
    trainstops = nameServer.getStopList(id);
    L1:
        for(;;){
            _Accept(~Train){
                break;
            } or _Accept(scanPassengers){
            } _Else {
                P.print(Printer::Kind::Train, id ,static_cast<char>(State::Arrive), id, (unsigned int)maxNumStudents-studentIdToPassengerInfo.size(),
                    (unsigned int)studentIdToPassengerInfo.size());
                    
                // wake up all the passengers who arrive at the destination
                // when student who calls the embark wakes up, they will call disembark
                unsigned int passengerSize = (unsigned int)studentIdToPassengerInfo.size();
                for(unsigned int i = 0; i < passengerSize; i++){
                    passengers.signalBlock();
                }

                // the train arrives first(let people call embark)
                unsigned int arrivingNum = trainstops[curStop]->arrive(id, static_cast<Direction>(id), 
                maxNumStudents-studentIdToPassengerInfo.size());

                // accept embark
                for(unsigned int i = 0; i < arrivingNum; i++){
                    try{
                        _Accept(~Train){
                            break L1;
                        } or _Accept(embark){
                            P.print(Printer::Kind::Train, id ,static_cast<char>(State::Embark), studentId, destStop);
                        }
                    } catch( uMutexFailure::RendezvousFailure & ) {} 
                    
                }
                curStop += nextStopAddr;
                curStop = (curStop+numStops)%numStops;
            }
        }
}
