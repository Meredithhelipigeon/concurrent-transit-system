#include "printer.h"
#include <iostream>
#include <cassert>

// allocate and print initial messages
Printer::Printer( unsigned int numStudents, unsigned int numTrains, unsigned int numStops, unsigned int numCouriers ) :
    numStudents(numStudents), numTrains(numTrains), numStops(numStops), numCouriers(numCouriers) {
    numTags = Kind::Train + numTrains * 2 + numStops + numStudents + numCouriers;
    // double numTrains since each train has a conductor
    printerTags = new Tag[numTags];
    // initial print of Kind and specifies the kind field of each tag
    for (unsigned int i = 0; i < numTags; i++) {
        if (i == Kind::Parent) {
            std::cout << "Parent";
            printerTags[i].kind = Kind::Parent;
        } else if (i == Kind::Groupoff) {
            std::cout << "\tGropoff";
            printerTags[i].kind = Kind::Groupoff;
        } else if (i == Kind::WATCardOffice) {
            std::cout << "\tWATOff";
            printerTags[i].kind = Kind::WATCardOffice;
        } else if (i == Kind::NameServer) {
            std::cout << "\tNames";
            printerTags[i].kind = Kind::NameServer;
        } else if (i == Kind::Timer) {
            std::cout << "\tTimer";
            printerTags[i].kind = Kind::Timer;
        } else if (i >= Kind::Train && i < Kind::Train + numTrains) {
            std::cout << "\tTrain" << i - Kind::Train;
            printerTags[i].kind = Kind::Train;
        } else if (i >= Kind::Train + numTrains && i < Kind::Train + 2 * numTrains) {
            std::cout << "\tCond" << i - (Kind::Train + numTrains);
            printerTags[i].kind = Kind::Conductor;
        } else if (i >= Kind::Train + 2 * numTrains && i < Kind::Train + 2 * numTrains + numStops) {
            std::cout << "\tStop" << i - (Kind::Train + 2 * numTrains);
            printerTags[i].kind = Kind::TrainStop;
        } else if (i >= Kind::Train + 2 * numTrains + numStops && i < Kind::Train + 2 * numTrains + numStops + numStudents) {
            std::cout << "\tStud" << i - (Kind::Train + 2 * numTrains + numStops);
            printerTags[i].kind = Kind::Student;
        } else {
            // i >= Kind::Train + 2 * numTrains + numStops + numStudents && i < numTags
            std::cout << "\tWCour" << i - (Kind::Train + 2 * numTrains + numStops + numStudents);
            // new line when last Kind
            if (i == numTags - 1) std::cout << std::endl;
            printerTags[i].kind = Kind::WATCardOfficeCourier;
        }
    }
    // line separator
    for (unsigned int i = 0; i < numTags; i++) {
        if (i > 0) std::cout << "\t";
        std::cout << "*******";
        if (i == numTags - 1) std::cout << std::endl;
    }
}

// deallocate and print complete message
Printer::~Printer() {
    flush();
    delete [] printerTags;
    std::cout << "***********************" << std::endl;
}

// print without lid is used by Parent, Groupoff, WATCardOffice, NameServer, Timer
// access tag in printerTags using index=kind
void Printer::print( Kind kind, char state ) {
    if (printerTags[kind].cacheInUse) flush();
    printerTags[kind].cacheInUse = true;
    printerTags[kind].state = state;
    printerTags[kind].value1 = -1;
}

void Printer::print( Kind kind, char state, unsigned int value1 ) {
    if (printerTags[kind].cacheInUse) flush();
    printerTags[kind].cacheInUse = true;
    printerTags[kind].state = state;
    printerTags[kind].value1 = value1;
}

void Printer::print( Kind kind, char state, unsigned int value1, unsigned int value2 ) {
    if (printerTags[kind].cacheInUse) flush();
    printerTags[kind].cacheInUse = true;
    printerTags[kind].state = state;
    printerTags[kind].value1 = value1;
    printerTags[kind].value2 = value2;
}

// print with lid calls getIdx to gets the index of corresponding tag in printerTags
void Printer::print( Kind kind, unsigned int lid, char state ) {
    unsigned int idx = getIdx(kind, lid);
    if (printerTags[idx].cacheInUse) flush();
    printerTags[idx].cacheInUse = true;
    printerTags[idx].state = state;
}

void Printer::print( Kind kind, unsigned int lid, char state, unsigned int value1 ) {
    unsigned int idx = getIdx(kind, lid);
    if (printerTags[idx].cacheInUse) flush();
    printerTags[idx].cacheInUse = true;
    printerTags[idx].state = state;
    printerTags[idx].value1 = value1;
}

void Printer::print( Kind kind, unsigned int lid, char state, unsigned int value1, unsigned int value2 ) {
    unsigned int idx = getIdx(kind, lid);
    if (printerTags[idx].cacheInUse) flush();
    printerTags[idx].cacheInUse = true;
    printerTags[idx].state = state;
    printerTags[idx].value1 = value1;
    printerTags[idx].value2 = value2;
}

void Printer::print( Kind kind, unsigned int lid, char state, unsigned int oid, unsigned int value1, unsigned int value2 ) {
    unsigned int idx = getIdx(kind, lid);
    if (printerTags[idx].cacheInUse) flush();
    printerTags[idx].cacheInUse = true;
    printerTags[idx].state = state;
    printerTags[idx].oid = oid;
    printerTags[idx].value1 = value1;
    printerTags[idx].value2 = value2;
}

void Printer::print( Kind kind, unsigned int lid, char state, unsigned int value1, char c ) {
    unsigned int idx = getIdx(kind, lid);
    if (printerTags[idx].cacheInUse) flush();
    printerTags[idx].cacheInUse = true;
    printerTags[idx].state = state;
    printerTags[idx].value1 = value1;
    printerTags[idx].c = c;
}

void Printer::print( Kind kind, unsigned int lid, char state, unsigned int value1, unsigned int value2, char c ) {
    unsigned int idx = getIdx(kind, lid);
    if (printerTags[idx].cacheInUse) flush();
    printerTags[idx].cacheInUse = true;
    printerTags[idx].state = state;
    printerTags[idx].value1 = value1;
    printerTags[idx].value2 = value2;
    printerTags[idx].c = c;
}

// getIdx returns index of corresponding object in printerTags
int Printer::getIdx(Kind kind, unsigned int lid) {
    int ret = -1;
    switch (kind) {
        case Kind::Train:
            ret = Kind::Train + lid;
            break;
        case Kind::Conductor:
            ret = Kind::Train + numTrains + lid;
            break;
        case Kind::TrainStop:
            ret = Kind::Train + 2 * numTrains + lid;
            break;
        case Kind::Student:
            ret = Kind::Train + 2 * numTrains + numStops + lid;
            break;
        case Kind::WATCardOfficeCourier:
            ret = Kind::Train + 2 * numTrains + numStops + numStudents + lid;
            break;
        default:
            std::cerr << "WRONG KIND" << std::endl;
            exit(1);
    }
    return ret;
}

// flush() will print all information from the buffer (printerTags) to output stream
// note: for easier debug purpose, we split the print statement into kinds corresponding
// to what have been described in the assignment spec
void Printer::flush() {
    // find where we will print up to (i.e. the first cacheInUse in reverse order)
    int idx = -1;
    for (int i = numTags - 1; i >= 0; i--) {
        if (printerTags[i].cacheInUse) {
            idx = i;
            break;
        }
    }
    // print for each Kind until index idx;
    for (int i = 0; i <= idx; i++) {
        if (i > 0) std::cout << "\t";
        // if tag is not in use, then continue to the next one
        if (!printerTags[i].cacheInUse) continue;
        // reset printerTags[i] to be not in used
        printerTags[i].cacheInUse = false;
        // print corresponding information stored in tag
        Tag curTag = printerTags[i];
        std::cout << curTag.state;
        switch (curTag.kind) {
            case Kind::Parent:
                // Ds,g
                if (curTag.state == 'D') {
                    assert(curTag.value1 != -1 && curTag.value2 != -1);
                    std::cout << curTag.value1 << "," << curTag.value2;
                }
                break;
            case Kind::Groupoff:
                // Dg
                if (curTag.state == 'D') {
                    assert(curTag.value1 != -1);
                    std::cout << curTag.value1;
                }
                break;
            case Kind::WATCardOffice:
                // Cs,a || Ts,a
                if (curTag.state == 'C' || curTag.state == 'T') {
                    assert(curTag.value1 != -1 && curTag.value2 != -1);
                    std::cout << curTag.value1 << "," << curTag.value2;
                }
                break;
            case Kind::NameServer:
                // Rs || Lt || L
                if (curTag.state == 'R' || curTag.state == 'L') {
                    // assert(curTag.value1 != -1)
                    if (curTag.value1!=-1) std::cout << curTag.value1;
                // Ts,t
                } else if (curTag.state == 'T') {
                    assert(curTag.value1 != -1 && curTag.value2 != -1);
                    std::cout << curTag.value1 << "," << curTag.value2;
                }
                break;
            case Kind::Timer:
                // tt
                if (curTag.state == 't') {
                    std::cout << curTag.value1;
                }
                break;
            case Kind::Train:
                // Ss,d
                if (curTag.state == 'S') {
                    assert(curTag.value1 != -1 && curTag.c != DUMMY);
                    std::cout << curTag.value1 << "," << curTag.c;
                // As,n,m
                } else if (curTag.state == 'A') {
                    assert(curTag.oid != -1 && curTag.value1 != -1 && curTag.value2 != -1);
                    std::cout << curTag.oid << "," << curTag.value1 << "," << curTag.value2;
                // Es,d
                } else if (curTag.state == 'E') {
                    assert(curTag.value1 != -1 && curTag.value2 != -1);
                    std::cout << curTag.value1 << "," << curTag.value2;
                }
                break;
            case Kind::Conductor:
                // en
                if (curTag.state == 'e') {
                    assert(curTag.value1 != -1);
                    std::cout << curTag.value1;
                }
                break;
            case Kind::TrainStop:
                // Aid,n,m
                if (curTag.state == 'A') {
                    assert(curTag.oid != -1 && curTag.value1 != -1 && curTag.value2 != -1);
                    std::cout << curTag.oid << "," << curTag.value1 << "," << curTag.value2;
                // Bc || Ds
                } else if (curTag.state == 'B' || curTag.state == 'D') {
                    assert(curTag.value1 != -1);
                    std::cout << curTag.value1;
                // Ws,d
                } else if (curTag.state == 'W') {
                    assert(curTag.value1 != -1 && curTag.c != DUMMY);
                    std::cout << curTag.value1 << "," << curTag.c;
                }
                break;
            case Kind::Student:
                // Sn || Ws || Et || Ds
                if (curTag.state == 'S' || curTag.state == 'W' || curTag.state == 'E' || curTag.state == 'D') {
                    assert(curTag.value1 != -1);
                    std::cout << curTag.value1;
                // Gc,b || Bc,b
                } else if (curTag.state == 'G' || curTag.state == 'B') {
                    assert(curTag.value1 != -1 && curTag.value2 != -1);
                    std::cout << curTag.value1 << "," << curTag.value2;
                // Ta,b,d
                } else if (curTag.state == 'T') {
                    assert(curTag.value1 != -1 && curTag.value2 != -1 && curTag.c != DUMMY);
                    std::cout << curTag.value1 << "," << curTag.value2 << "," << curTag.c;
                }
                break;
            case Kind::WATCardOfficeCourier:
                // ts,a || Ts,a
                if (curTag.state == 't' || curTag.state == 'T') {
                    assert(curTag.value1 != -1 && curTag.value2 != -1);
                    std::cout << curTag.value1 << "," << curTag.value2;
                // Ls
                } else if (curTag.state == 'L') {
                    assert(curTag.value1 != -1);
                    std::cout << curTag.value1;
                }
                break;
            default:
                break;
        }
        // new line
        if (i == idx) std::cout << std::endl;
    }
}
