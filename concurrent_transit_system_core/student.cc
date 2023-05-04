#include "student.h"
#include "trainstop.h"
#include "cardoffice.h"
#include "groupoff.h"
#include "nameserver.h"
#include "printer.h"

Student::Student( Printer & prt, NameServer & nameServer, WATCardOffice & cardOffice, Groupoff & groupoff, 
    unsigned int id, unsigned int numStops, unsigned int stopCost, unsigned int maxStudentDelay, unsigned int maxStudentTrips ):
    id{id}, maxStudentDelay{maxStudentDelay}, numStops{numStops}, stopCost{stopCost},
    P{prt}, nameServer{nameServer}, cardOffice{cardOffice}, groupoff{groupoff} {
        trainTrip = (unsigned int) prng(1,maxStudentTrips);
        // get a WATCard from cardOffice with maximum cost of a trip
        maxCostOfTrip = stopCost*(numStops/2);
        FWATCard = cardOffice.create(id, maxCostOfTrip);
        // get a Giftcard from Groupoff with maximum cost of a trip
        GiftCard = groupoff.giftCard();
    }

Student::~Student(){
    // delete the WARCard pointer
    try{
        delete FWATCard(); 
    } catch(WATCardOffice::Lost &){}
}

// checkDirection() is a helper function such that checks which direction-train the student should take
void Student::checkDirection(){
    unsigned int clockwiseStops = (endStop - startStop + numStops) % numStops;
    tripStops = std::min(clockwiseStops, numStops-clockwiseStops);
    if (clockwiseStops !=  tripStops) direction = Train::CounterClockwise;
    else direction = Train::Clockwise;
}

void Student::main(){
    P.print(Printer::Kind::Student, id ,static_cast<char>(State::Start), trainTrip);
    for(unsigned int i = 0; i<trainTrip; i++){
        // before each trip is started, the student delays a random amount of time [0, maxStudentDelay]
        yield(prng(0,maxStudentDelay));

        // the first startStop is [0, numStops)
        if (i==0) startStop = prng(numStops);
        // pick a new random distinct endStop
        endStop = prng(numStops);
        while (startStop==endStop){
            endStop = prng(numStops);
        }

        checkDirection();
        if (direction==Train::Clockwise) {
            P.print(Printer::Kind::Student, id ,static_cast<char>(State::Trip), startStop, endStop, static_cast<char>(State::Clockwise));
        } else {
            P.print(Printer::Kind::Student, id ,static_cast<char>(State::Trip), startStop, endStop, static_cast<char>(State::CounterClockwise));
        }


        unsigned int ticketCosts = tripStops *  stopCost;
        bool ifAvoidPaying = false;

        if (tripStops==1){
            // 50% chance that the student will avoid paying
            if (prng(2)==0) ifAvoidPaying = true;
        } else {
            // 30% chance that the student will avoild paying
            if (prng(10)<3) ifAvoidPaying = true;
        }

        TrainStop * startStopLocation = nameServer.getStop(id, startStop);
        TrainStop * endStopLocation = nameServer.getStop(id, endStop);
        Train * train;
        WATCard * validCard = NULL;

        if (!ifAvoidPaying){
            _When (!ifGiftUsed) _Select( GiftCard ){
                P.print(Printer::Kind::Student, id ,static_cast<char>(State::GiftTicket), ticketCosts, GiftCard()->getBalance()-ticketCosts);
                startStopLocation->buy(tripStops, *GiftCard());
                P.print(Printer::Kind::Student, id ,static_cast<char>(State::Wait), startStop);
                train = startStopLocation->wait(id, direction);
                P.print(Printer::Kind::Student, id ,static_cast<char>(State::Embark), train->getId());
                train->embark(id, endStop, *GiftCard());
                // reset GiftCard since gift card can only be used once
                ifGiftUsed = true;
                GiftCard.reset();
            } or _Select( FWATCard ) {
                // This step makes sure that the loaded card is valid now
                L2:
                    for(;;){
                        try{
                            if (FWATCard()->getBalance()>=ticketCosts){
                                validCard = FWATCard();
                                break L2;
                            }
                            FWATCard = cardOffice.transfer(id,ticketCosts-FWATCard()->getBalance()+maxCostOfTrip,FWATCard());
                        } catch(WATCardOffice::Lost &){
                            // if the card is lost; ask for creating a new one
                            FWATCard = cardOffice.create(id, maxCostOfTrip);
                            P.print(Printer::Kind::Student, id ,static_cast<char>(State::LostCrad));
                        }
                    }
                // wait until the wat card's money is transferred
                P.print(Printer::Kind::Student, id ,static_cast<char>(State::WatCardTicket), ticketCosts, validCard->getBalance()-ticketCosts);
                startStopLocation->buy(tripStops, *validCard);
                P.print(Printer::Kind::Student, id ,static_cast<char>(State::Wait), startStop);
                train = startStopLocation->wait(id, direction);
                P.print(Printer::Kind::Student, id ,static_cast<char>(State::Embark), train->getId());
                train->embark(id, endStop, *validCard);

            }
        } else {
            P.print(Printer::Kind::Student, id ,static_cast<char>(State::FreeRide));

            // This step makes sure that the loaded card is valid now
            L3:
                for(;;){
                    try{
                        validCard = FWATCard();
                        break L3;
                    } catch(WATCardOffice::Lost &){
                        // if the card is lost; ask for creating a new one
                        FWATCard = cardOffice.create(id, maxCostOfTrip);
                        P.print(Printer::Kind::Student, id ,static_cast<char>(State::LostCrad));
                    }
                }
            P.print(Printer::Kind::Student, id ,static_cast<char>(State::Wait), startStop);
            train = startStopLocation->wait(id, direction);
            P.print(Printer::Kind::Student, id ,static_cast<char>(State::Embark), train->getId());
            // This step makes sure that the new-created card could be taken to the embark
            try {
                train->embark(id, endStop, *validCard);
            } catch(Train::Ejected &){
                // if the conductor find out this person, he will be enjected
                // the student will terminate his trip
                P.print(Printer::Kind::Student, id ,static_cast<char>(State::Ejected));
                break;
            }
        }
        P.print(Printer::Kind::Student, id ,static_cast<char>(State::Disembark), endStop);
        endStopLocation->disembark(id);
        L4:
            for(;;){
                try{
                    // student card will be resetPOP status
                    FWATCard()->resetPOP();
                    break L4;
                // This step makes sure that the new-created card could be taken to the reset if LOST happens
                } catch(WATCardOffice::Lost &){
                    // if the card is lost; ask for creating a new one
                    FWATCard = cardOffice.create(id, maxCostOfTrip);
                    P.print(Printer::Kind::Student, id ,static_cast<char>(State::LostCrad));
                } 
            }
        startStop = endStop;
    }
    P.print(Printer::Kind::Student, id ,static_cast<char>(State::Finished));
}
