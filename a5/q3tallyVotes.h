#ifndef __Q3_TALLY_H__
#define __Q3_TALLY_H__

_Monitor Printer;

#if defined( IMPLTYPE_EXT )
_Monitor TallyVotes {
#elif defined( IMPLTYPE_INT )
_Monitor TallyVotes {
    uCondition voting;
#elif defined( IMPLTYPE_INTB )
_Monitor TallyVotes {
    uCondition bench;
    unsigned int tickets;
    unsigned int serving;
    
    void wait();                    // barging version of wait
    void signalAll();                // unblock all waiting tasks
#elif defined( IMPLTYPE_AUTO )
#include "AutomaticSignal.h"
_Monitor TallyVotes {
    AUTOMATIC_SIGNAL;
    bool allVoted;
#elif defined( IMPLTYPE_TASK )
_Task TallyVotes {
    unsigned int curId;

    void main();
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
#if defined( IMPLTYPE_TASK )
  private:
    Tour curBallot;
#endif
};


#endif
