#pragma once

#include <uFuture.h>
#include <iostream>

class WATCard {
private:
    bool POP;                                           // proof of purchase
    unsigned int balance;                               // balance is set to 0 at creation

    friend _Task TrainStop;
    void markPaid();

    WATCard( const WATCard & ) = delete;				// prevent copying
    WATCard & operator=( const WATCard & ) = delete;

public:
    typedef Future_ISM<WATCard *> FWATCard;				// future watcard pointer
    WATCard();
    ~WATCard();
    void deposit( unsigned int amount );
    void withdraw( unsigned int amount );
    unsigned int getBalance();
    bool paidForTicket();
    void resetPOP();
}; // WATCard
