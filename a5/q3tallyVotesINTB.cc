#include "q3tallyVotes.h"
#include "q3printer.h"

void TallyVotes::wait() {
    bench.wait();                            // wait until signalled
    while ( rand() % 5 == 0 ) {                // multiple bargers allowed
        _Accept( vote ) {                    // accept barging callers
        } _Else {                            // do not wait if no callers
        } // _Accept
    } // while
}

void TallyVotes::signalAll() {                // also useful
    while ( ! bench.empty() ) bench.signal();        // drain the condition
}


TallyVotes::Tour TallyVotes::vote( unsigned int id, Tour ballot ) {
	tickets += 1;
	unsigned int ticket = tickets;
	while (ticket > serving)  {
		printer.print(id, Voter::States::Barging);
		wait();
	}
	
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
		wait(); 
		// woke up
		numWaiting -= 1;
		printer.print(id, Voter::States::Unblock, numWaiting);
	} else {
		printer.print(id, Voter::States::Complete);
		numWaiting -= 1;
		signalAll();
	}

	Tour result = picture > statue ? Picture : Statue;
	printer.print(id, Voter::States::Finished, result);
	
	if (numWaiting == 0) {
		picture = 0;
		statue = 0;
		serving += groupSize;
		signalAll();
	}
	
	return result;
}


TallyVotes::TallyVotes( unsigned int group, Printer &printer )
: tickets(0)
, serving(group)
, printer(printer)
, groupSize(group)
, numWaiting(0)
, picture(0)
, statue(0) {
}
