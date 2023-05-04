#include "timer.h"
#include "printer.h"

Timer::Timer( Printer & prt, NameServer & nameServer, unsigned int timerDelay ):
    P{ prt }, nameServer { nameServer }, timeDelay { timerDelay } {}

void Timer::main(){
    P.print(Printer::Kind::Timer, static_cast<char>(State::Start));
    unsigned int time = 0;
    trainstops = nameServer.getStopList();
    numStops = nameServer.getNumStops();
    for(;;){
        _Accept(~Timer){
            break;
        } _Else{
            yield(timeDelay);
            P.print(Printer::Kind::Timer, static_cast<char>(State::Tick), time);
            time += 1;
            for(unsigned int i = 0; i < numStops; i++){
                trainstops[i]->tick();
            }
        }
    }
    P.print(Printer::Kind::Timer, static_cast<char>(State::Finished));
}

Timer::~Timer(){}
