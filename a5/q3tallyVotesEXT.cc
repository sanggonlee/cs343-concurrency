#include "q3tallyVotes.h"
#include "q3printer.h"

TallyVotes::Tour TallyVotes::vote( unsigned int id, Tour ballot ) {
	printer.print(id, Voter::States::Vote, ballot);
	switch (ballot) {
		case Picture:
			picture += 1;
			break;
		case Statue:
			statue += 1;
	}
	numWaiting += 1;
	if (numWaiting < groupSize) {
		printer.print(id, Voter::States::Block, numWaiting);
		_Accept(vote) 
		// woke up
		numWaiting -= 1;
		printer.print(id, Voter::States::Unblock, numWaiting);
	} else {
		printer.print(id, Voter::States::Complete);
		numWaiting -= 1;
	}

	Tour result = picture > statue ? Picture : Statue;
	printer.print(id, Voter::States::Finished, result);
	
	if (numWaiting == 0) {
		picture = 0;
		statue = 0;
	}
	
	return result;
}


TallyVotes::TallyVotes( unsigned int group, Printer &printer )
: printer(printer)
, groupSize(group)
, numWaiting(0)
, picture(0)
, statue(0) {
}
