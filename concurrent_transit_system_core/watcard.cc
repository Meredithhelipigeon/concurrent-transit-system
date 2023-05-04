#include "watcard.h"

// set POP to false and balance to false at creation
WATCard::WATCard() : POP(false), balance(0) {}

// default destructor
WATCard::~WATCard() {}

// makrPaid is called after withdraw to declare POP true
void WATCard::markPaid() {
    // declare POP true after fare amount is deducted
    POP = true;
}

// deposit to the account
void WATCard::deposit(unsigned int amount) {
    balance += amount;
}

// withdraw from the account
void WATCard::withdraw(unsigned int amount) {
    // make sure amount is not greater than balance
    if (amount > balance) std::cerr << "NOT ENOUGH MONEY" << std::endl;
    balance -= amount;
}

// display current balance
unsigned int WATCard::getBalance() {
    return balance;
}

// display POP (whether a fare is purchased)
bool WATCard::paidForTicket() {
    return POP;
}

// reset POP when arrive
void WATCard::resetPOP() {
    POP = false;
}
