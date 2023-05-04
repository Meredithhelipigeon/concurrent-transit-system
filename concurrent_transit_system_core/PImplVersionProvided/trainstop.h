#pragma once

#include "global.h"
#include "train.h"

_Task TrainStop {
	class PImpl; // *** Replace these two lines with your own implementation.
	PImpl *pimpl;

	void main();
  public:
	_Event Funds {										// Thrown when there are insufficient funds on the card to buy a ticket.
	  public:
		unsigned int amount;
		Funds( unsigned int amt );
	};

	TrainStop( Printer & prt, NameServer & nameServer, unsigned int id, unsigned int stopCost );
	~TrainStop();
	_Nomutex unsigned int getId() const;

	// Student uses card to buy a ticket for numStops stops.
	void buy( unsigned int numStops, WATCard & card );
	
	// Student waits for a train traveling in the specified direction.
	Train * wait( unsigned int studentId, Train::Direction direction );
	
	// Student getting off the train at this stop "disembarks" at this stop.				
	void disembark( unsigned int studentId );
	
	// Timer notifies stop that timer ticked.	  
	void tick();

	// Train tells stop in which direction it is traveling, and the maximum number of students it can take.
	// It then blocks until the timer ticks over. Returns the minimum of what it can take, and the number
	// of students waiting.
	unsigned int arrive( unsigned int trainId, Train::Direction direction, unsigned int maxNumStudents );
};

// Local Variables: //
// mode: c++ //
// tab-width: 4 //
// compile-command: "make" //
// End: //
