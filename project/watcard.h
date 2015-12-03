#ifndef __WATCARD_H__
#define __WATCARD_H__

#include <uFuture.h>

class WATCard {
	unsigned int balance;

    WATCard( const WATCard & );            // prevent copying
    WATCard &operator=( const WATCard & );
  public:
    typedef Future_ISM<WATCard *> FWATCard; // future watcard pointer
    WATCard();
    ~WATCard();
    void deposit( unsigned int amount );
    void withdraw( unsigned int amount );
    unsigned int getBalance();
};

#endif
