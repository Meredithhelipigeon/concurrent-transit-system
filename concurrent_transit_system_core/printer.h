#pragma once

_Monitor Printer {
public:
    enum Kind { Parent, Groupoff, WATCardOffice, NameServer, Timer, Train, Conductor, TrainStop, Student, WATCardOfficeCourier };

    Printer( unsigned int numStudents, unsigned int numTrains, unsigned int numStops, unsigned int numCouriers );
    ~Printer();
    void print( Kind kind, char state );
    void print( Kind kind, char state, unsigned int value1 );
    void print( Kind kind, char state, unsigned int value1, unsigned int value2 );
    void print( Kind kind, unsigned int lid, char state );
    void print( Kind kind, unsigned int lid, char state, unsigned int value1 );
    void print( Kind kind, unsigned int lid, char state, unsigned int value1, unsigned int value2 );
    // oid will be the station id or train id
    void print( Kind kind, unsigned int lid, char state, unsigned int oid, unsigned int value1, unsigned int value2 );
    void print( Kind kind, unsigned int lid, char state, unsigned int value1, char c );
    void print( Kind kind, unsigned int lid, char state, unsigned int value1, unsigned int value2, char c );

private:
    // tag is the general form for storing information of each print statement
    // we set the fields as needed. Otherwise, they remain default dummy values
    // kind specifies what type of print statement is used
    static const char DUMMY = 'Z';
    typedef struct Tag {
        Kind kind       = Parent;
        char state      = DUMMY;
        // oid, value1, value2, or c are corresponding values shown on assignment spec
        // when calling print, followings are filled as much in order of oid to c
        int oid         = -1;
        int value1      = -1;
        int value2      = -1;
        char c          = DUMMY;
        bool cacheInUse = false;
    } Tag;
    unsigned int numStudents;
    unsigned int numTrains;
    unsigned int numStops;
    unsigned int numCouriers;
    unsigned int numTags;
    Tag *printerTags;               // first 5 elements are reserved for Parent, Groupoff, WATCardOffice, NameServer, Timer
    void flush();
    int getIdx(Kind kind, unsigned int lid);
}; // Printer

// Local Variables: //
// mode: c++ //
// tab-width: 4 //
// compile-command: "make" //
// End: //
