#pragma once

struct ConfigParms {
	unsigned int stopCost;								// amount to charge per train stop
	unsigned int numStudents;							// number of students to create
	unsigned int numStops;								// number of train stops; minimum of 2
	unsigned int maxNumStudents;						// maximum students each train can carry
	unsigned int timerDelay;							// length of time between each tick of the timer
	unsigned int maxStudentDelay;						// maximum random student delay between trips
	unsigned int maxStudentTrips;						// maximum number of train trips each student takes
	unsigned int groupoffDelay;							// length of time between initializing gift cards
	unsigned int conductorDelay;						// length of time between checking on passenger POPs
	unsigned int parentalDelay;							// length of time between cash deposits
	unsigned int numCouriers;							// number of WATCard office couriers in the pool
}; // ConfigParms

void processConfigFile( const char * configFile, ConfigParms & cparms );

// Local Variables: //
// mode: c++ //
// tab-width: 4 //
// compile-command: "make" //
// End: //
