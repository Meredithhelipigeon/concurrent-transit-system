#include "trainstop.h"
#include "watcard.h"
#include "printer.h"
#include "nameserver.h"

TrainStop::TrainStop( Printer & prt, NameServer & nameServer, unsigned int id, unsigned int stopCost ):
    P{ prt }, nameServer{ nameServer }, id{ id }, stopCost{ stopCost }{}

_Nomutex unsigned int TrainStop::getId() const{
    return id;
}

void TrainStop::buy( unsigned int numStops, WATCard & card ){
    TrainStop::curFareCost = numStops*stopCost;
    if (card.getBalance()<curFareCost){
        _Throw Funds((unsigned int)curFareCost-card.getBalance());
    }
    card.withdraw(curFareCost);
    card.markPaid();
}

Train * TrainStop::wait( unsigned int studentId, Train::Direction direction ){
    TrainStop::studentId = studentId;
    TrainStop::direction = direction;

    // wait on different benches given the direction
    if (direction==Train::Clockwise) {
        clockwiseBench.wait(studentId);
        clockwiseNum += 1;
    } else {
        counterClockwiseBench.wait(studentId);
        counterClockwiseNum += 1;
    }
    return curTrain;
}

void TrainStop::disembark( unsigned int studentId ){
    disembarkBench.wait();
    P.print(Printer::Kind::TrainStop, id ,static_cast<char>(State::Disembark), studentId);
}

void TrainStop::tick(){};

unsigned int TrainStop::arrive( unsigned int trainId, Train::Direction direction, unsigned int maxNumStudents ){
    TrainStop::curTrainId = trainId;
    TrainStop::ifClockwise = (direction==Train::Clockwise);
    TrainStop::maxNumStudents = maxNumStudents;
    TrainStop::curTrain = static_cast<Train*>(&uThisTask());
    // the train will start again when the tick is accepted
    arriversBench.wait();

    int ret;
    if (direction==Train::Clockwise) {
        ret = std::min(clockwiseNum, maxNumStudents);
        clockwiseNum -= ret;
    } else {
        ret = std::min(counterClockwiseNum, maxNumStudents);
        counterClockwiseNum -= ret;
    }
    return ret;
}

void TrainStop::main(){
    P.print(Printer::Kind::TrainStop, id ,static_cast<char>(State::Start));
    nameServer.registerStop(id);
    for(;;){
        try {
            _Accept(~TrainStop){
                break;
            } or _Accept(disembark){
            } or _Accept(buy){
                P.print(Printer::Kind::TrainStop, id ,static_cast<char>(State::Bought), curFareCost);
            } or _Accept(wait){
                if (direction == Train::Clockwise) P.print(Printer::Kind::TrainStop, id ,static_cast<char>(State::Wait), studentId, static_cast<char>(State::Clockwise));
                else P.print(Printer::Kind::TrainStop, id ,static_cast<char>(State::Wait), studentId, static_cast<char>(State::CounterClockwise));
            } or  _Accept(tick){
                P.print(Printer::Kind::TrainStop, id ,static_cast<char>(State::Tick));

                // there could be two trains waiting to be unblocked so that we use bench here
                while(!arriversBench.empty()) arriversBench.signalBlock();
            } or _Accept(arrive){
                if (ifClockwise) P.print(Printer::Kind::TrainStop, id ,static_cast<char>(State::Arrive), curTrainId, maxNumStudents, clockwiseNum);
                else P.print(Printer::Kind::TrainStop, id ,static_cast<char>(State::Arrive), curTrainId, maxNumStudents, counterClockwiseNum);

                // wake up the passengers so that they could call embark
                for(unsigned int i=0; i<maxNumStudents;i++){
                    if (ifClockwise){
                        if (clockwiseBench.empty()) break;
                        clockwiseBench.signalBlock();
                    } else {
                        if (counterClockwiseBench.empty()) break;
                        counterClockwiseBench.signalBlock();
                    }
                }
                // let people disembark
                while(!disembarkBench.empty()) disembarkBench.signalBlock();
            }
        } catch (uMutexFailure::RendezvousFailure &) { 
        }
    }
    P.print(Printer::Kind::TrainStop, id ,static_cast<char>(State::Finished));
}

TrainStop::~TrainStop(){}
