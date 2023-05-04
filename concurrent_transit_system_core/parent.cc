#include "parent.h"
#include "bank.h"
#include "printer.h"
#include <algorithm>

Parent::Parent( Printer & prt, Bank & bank, unsigned int numStudents, unsigned int parentalDelay, unsigned int maxTripCost ):
    P{ prt }, bank{ bank }, numStudents{ numStudents }, parentDelay { parentalDelay }, maxTripCost{ maxTripCost } {}

void Parent::main(){
    P.print(Printer::Kind::Parent, static_cast<char>(State::Start));
    for(;;){
        _Accept(~Parent){
            break;
        } _Else {
            yield(parentDelay);
            int student = prng(numStudents);
            int amount = std::max(1, (int)(maxTripCost * (prng(1,3) / 3)));
            P.print(Printer::Kind::Parent, static_cast<char>(State::Deposit), student, amount);
            bank.deposit(student, amount);
        }
    }
    P.print(Printer::Kind::Parent, static_cast<char>(State::Finished));
}

Parent::~Parent() {}
