#ifndef __Q2_TALLY_H__
#define __Q2_TALLY_H__

_Monitor Printer;

#if defined( IMPLTYPE_MC )            // mutex/condition solution
class TallyVotes {
	uOwnerLock mutexLock;
	uCondLock curGroupCondLock;
	uCondLock syncGroupCondLock;
#elif defined( IMPLTYPE_BAR )        // barrier solution
#include <uBarrier.h>
_Cormonitor TallyVotes : public uBarrier {
	void main();
	void block();
#elif defined( IMPLTYPE_SEM )        // semaphore solution
#include <uSemaphore.h>
class TallyVotes {
	uSemaphore lock;
	uSemaphore groupSem;
	uSemaphore syncGroupSem;
	unsigned int numVoted;
#else
    #error unsupported voter type
#endif
    // common declarations
    Printer &printer;
    unsigned int groupSize;
    unsigned int numWaiting;
    unsigned int picture;
    unsigned int statue;
  public:                            // common interface
    TallyVotes( unsigned int group, Printer &printer );
    enum Tour { Picture, Statue };
    Tour vote( unsigned int id, Tour ballot );
};


#endif
