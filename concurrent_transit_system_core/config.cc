#include "config.h"

void processConfigFile( const char * configFile, ConfigParms & cparms ){
        ifstream myReadFile(configFile);
        string line;
    
        while (getline(myReadFile, line)) {
            string name;
            stringstream ss(line);
            ss >> name;
    
            if (name == "StopCost") {
                ss >> cparms.stopCost;
            } else if (name == "NumStudents") {
                ss >> cparms.numStudents;
            } else if (name == "NumStops") {
                ss >> cparms.numStops;
            } else if (name == "MaxNumStudents") {
                ss >> cparms.maxNumStudents;
            } else if (name == "TimerDelay") {
                ss >> cparms.timerDelay;
            } else if (name == "MaxStudentDelay") {
                ss >> cparms.maxStudentDelay;
            } else if (name == "MaxStudentTrips") {
                ss >> cparms.maxStudentTrips;
            } else if (name == "GroupoffDelay") {
                ss >> cparms.groupoffDelay;
            } else if (name == "ConductorDelay") {
                ss >> cparms.conductorDelay;
            } else if (name == "ParentalDelay") {
                ss >> cparms.parentalDelay;
            } else if (name == "NumCouriers") {
                ss >> cparms.numCouriers;
            }
        }
}