#pragma once

#include <uFuture.h>

class WATCard {
    friend _Task TrainStop;
    
    class PImpl; // *** Replace these two lines with your own implementation.
    PImpl * pimpl;

    WATCard( const WATCard & ) = delete;				// prevent copying
    WATCard & operator=( const WATCard & ) = delete;

    void markPaid();
    
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
