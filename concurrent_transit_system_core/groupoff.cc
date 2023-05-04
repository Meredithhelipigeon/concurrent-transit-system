#include "groupoff.h"
#include "printer.h"

Groupoff::Groupoff( Printer & prt, unsigned int numStudents, unsigned int maxTripCost, unsigned int groupoffDelay ):
    P { prt }, numStudents { numStudents }, maxTripCost { maxTripCost }, groupoffDelay { groupoffDelay }{
        watCards = new WATCard::FWATCard[numStudents];
        cards = new WATCard*[numStudents];
        watCardsIndex = new int[numStudents];
        for (unsigned int i = 0; i < numStudents; i++) {
            watCardsIndex[i]=i;
        }
    }

WATCard::FWATCard Groupoff::giftCard(){
    return watCards[assignedNum];
}

void Groupoff::main(){
    P.print(Printer::Kind::Groupoff, static_cast<char>(State::Start));
    for(;;){
        _Accept( ~Groupoff ){
            break;
        } or _When(assignedNum<numStudents) _Accept( giftCard ){
            yield(groupoffDelay);
            int randomIndex = prng(assignedNum, numStudents-1);
            int cardIndex = watCardsIndex[randomIndex]; // real card index that groupoff assign values
            // switch index in the array
            watCardsIndex[randomIndex] = watCardsIndex[assignedNum];
            watCardsIndex[assignedNum] = cardIndex;
            // create a new WAT card and delivery to WATCard
            WATCard * watcard = new WATCard();
            P.print(Printer::Kind::Groupoff, static_cast<char>(State::Deposit), maxTripCost);
            cards[assignedNum]=watcard;
            watcard->deposit(maxTripCost);
            watCards[cardIndex].delivery(watcard);
            assignedNum += 1;
        } _When(assignedNum==numStudents) _Else {
            P.print(Printer::Kind::Groupoff, static_cast<char>(State::Finished));
            // wait for the destructor now
            _Accept( ~Groupoff ){
                break;
            }
        }
    }
}

Groupoff::~Groupoff(){
    // delete all the cards that we have sent
    for (int i = assignedNum-1; i >= 0; i--) {
        delete cards[i];
    }
    delete [] cards;
    delete [] watCards;
    delete [] watCardsIndex;
}
