#include "bank.h"

Bank::Bank( unsigned int numStudents): numStudents{ numStudents } {
    studentIdToMoney = new int[numStudents]();
    studentIdToWaitChair = new uCondition[numStudents]();
}

void Bank::deposit( unsigned int id, unsigned int amount ){
    studentIdToMoney[id] += amount;
    studentIdToWaitChair[id].signal();
}

void Bank::withdraw( unsigned int id, unsigned int amount ){
    while ((unsigned int)studentIdToMoney[id] < amount){
        studentIdToWaitChair[id].wait();
    }
    studentIdToMoney[id] -= amount;
}

Bank::~Bank(){
    delete []studentIdToMoney;
    delete []studentIdToWaitChair;
}
