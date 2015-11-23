#ifndef __Q2_VOTER_H__
#define __Q2_VOTER_H__

#include "MPRNG.h"
#include "q2tallyVotes.h"

extern MPRNG mprng;

_Task Voter {
	TallyVotes &tallier;
	Printer &printer;
	unsigned int id;
	
	void main();
  public:
    enum States { Start = 'S', Vote = 'V', Block = 'B', Unblock = 'U',
                   Complete = 'C', Finished = 'F' };
    Voter( unsigned int id, TallyVotes &voteTallier, Printer &printer );
};

#endif
