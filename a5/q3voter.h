#ifndef __Q3_VOTER_H__
#define __Q3_VOTER_H__

#include "MPRNG.h"
#include "q3tallyVotes.h"

extern MPRNG mprng;

_Task Voter {
	TallyVotes &tallier;
	Printer &printer;
	unsigned int id;
	
	void main();
  public:
    enum States { Start = 'S', Vote = 'V', Block = 'B', Unblock = 'U', 
    				Barging = 'b', Complete = 'C', Finished = 'F' };
    Voter( unsigned int id, TallyVotes &voteTallier, Printer &printer );
};

#endif
