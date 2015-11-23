#include "q3tallyVotes.h"
#include "q3printer.h"

#undef AUTOMATIC_SIGNAL

#include <iostream>
using namespace std;

TallyVotes::Tour TallyVotes::vote( unsigned int id, Tour ballot ) {
	printer.print(id, Voter::States::Vote, ballot);
	switch (ballot) {
		case Picture:
			picture += 1;
			break;
		case Statue:
			statue += 1;
	}
	
	if (numWaiting+1 == groupSize) {
		printer.print(id, Voter::States::Complete);
		allVoted = true;
	}

	WAITUNTIL(allVoted,
		numWaiting += 1; printer.print(id, Voter::States::Block, numWaiting),
		numWaiting -= 1; printer.print(id, Voter::States::Unblock, numWaiting));

	Tour result = picture > statue ? Picture : Statue;
	printer.print(id, Voter::States::Finished, result);
	
	if (numWaiting == 0) {
		picture = 0;
		statue = 0;
		allVoted = false;
	}
	
	RETURN(result);
}


TallyVotes::TallyVotes( unsigned int group, Printer &printer )
: allVoted(false)
, printer(printer)
, groupSize(group)
, numWaiting(0)
, picture(0)
, statue(0) {
}
