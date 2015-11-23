#include "q2tallyVotes.h"
#include "q2printer.h"

/*
 * Process the vote
 * 1. Wait if the current group is full and tallying stage started
 * 2. Print the voter's vote choice
 * 3(a). If this voter is not the last one to join the group, print
 *       Block and wait for voting to finish. When woken up, print
 *       Unblock
 * 3(b). If this is the last voter to join the group, print Complete
 *       and wake other waiting voters
 * 4. Compute the vote result and print Finished
 * 5. If this voter is the last one to finish, reset tally and wake
 *    the voters waiting for the next group to be started
 */
TallyVotes::Tour TallyVotes::vote( unsigned int id, Tour ballot ) {
#if defined( IMPLTYPE_MC )
	mutexLock.acquire();
	if (numWaiting >= groupSize) {
		syncGroupCondLock.wait(mutexLock);
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
		curGroupCondLock.wait(mutexLock);
		// woke up
		numWaiting -= 1;
		printer.print(id, Voter::States::Unblock, numWaiting);
	} else {
		printer.print(id, Voter::States::Complete);
		numWaiting -= 1;
		curGroupCondLock.broadcast();
	}
	
	Tour result = picture > statue ? Picture : Statue;
	printer.print(id, Voter::States::Finished, result);
	
	if (numWaiting == 0) {
		syncGroupCondLock.broadcast();
		picture = 0;
		statue = 0;
	}
	
	mutexLock.release();
	
	return result;

#elif defined( IMPLTYPE_BAR )
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
		block();
		numWaiting -= 1;
		printer.print(id, Voter::States::Unblock, numWaiting);
	} else if (numWaiting == groupSize) {
		printer.print(id, Voter::States::Complete);
		block();
		numWaiting -= 1;
	}
	
	Tour result = picture > statue ? Picture : Statue;
	printer.print(id, Voter::States::Finished, result);
	
	if (numWaiting == 0) {
		picture = 0;
		statue = 0;
	}
	
	return result;

#elif defined( IMPLTYPE_SEM )
	lock.P();
	if (numWaiting >= groupSize || numVoted == groupSize) {
		syncGroupSem.P(lock);
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
	numVoted += 1;
	if (numWaiting < groupSize) {
		printer.print(id, Voter::States::Block, numWaiting);
		groupSem.P(lock);
		// woke up
		groupSem.V();	// successively wake someone else
		numWaiting -= 1;
		printer.print(id, Voter::States::Unblock, numWaiting);
	} else if (numWaiting == groupSize) {
		printer.print(id, Voter::States::Complete);
		groupSem.V();
		numWaiting -= 1;
	}
	
	Tour result = picture > statue ? Picture : Statue;
	printer.print(id, Voter::States::Finished, result);
	
	if (numWaiting == 0) {
		groupSem.P();	// reset to closed
		syncGroupSem.V();
		picture = 0;
		statue = 0;
		numVoted = 0;
		lock.V();
	}
	
	return result;
#endif
}

/*
 * Constructor for TallyVotes
 */
TallyVotes::TallyVotes( unsigned int group, Printer &printer ) :
#if defined( IMPLTYPE_MC )
#elif defined( IMPLTYPE_BAR )
  uBarrier(group),
#elif defined( IMPLTYPE_SEM )
  lock(1),
  groupSem(0),
  syncGroupSem(0),
  numVoted(0),
#endif
  printer(printer),
  groupSize(group),
  numWaiting(0),
  picture(0),
  statue(0) {
}


#if defined( IMPLTYPE_BAR )
/*
 * Override block() method to prevent compile error
 */
void TallyVotes::block() {
	uBarrier::block();
}

/*
 * Override main() method to prevent compile error
 */
void TallyVotes::main() {
	uBarrier::main();
}
#endif









