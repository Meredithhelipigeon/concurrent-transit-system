#include "conductor.h"
#include "train.h"
#include "printer.h"

Conductor::Conductor( Printer & prt, unsigned int id, Train * train, unsigned int delay ):
    P{ prt }, id { id }, train{ train }, delay{ delay } {}

void Conductor::main(){
    P.print(Printer::Kind::Conductor, id, static_cast<char>(State::Start));
    for(;;){
        _Accept(~Conductor){
            break;
        } _Else {
            //  yielding the CPU conductorDelay times
            yield(delay);
            P.print(Printer::Kind::Conductor, id, static_cast<char>(State::Check));
            train->scanPassengers();
        }
    }
    P.print(Printer::Kind::Conductor, id,  static_cast<char>(State::Finished));
}

Conductor::~Conductor(){}
