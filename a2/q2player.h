#ifndef __PLAYER_H__
#define __PLAYER_H__
#include "q2printer.h"
#include "PRNG.h"

extern PRNG prng;

_Coroutine Player {
    // YOU MAY ADD PRIVATE MEMBERS
    unsigned int id;
    Printer *printer;
    Player *left;
    Player *right;
    static unsigned int numRemainingPlayers;
    static unsigned int numRemainingCards;
    
    void main();
  public:
    enum { DEATH_DECK_DIVISOR = 5 };
    static void players( unsigned int num );
    Player( Printer &printer, unsigned int id );
    void start( Player &lp, Player &rp );
    void play( unsigned int deck );
};

#endif
