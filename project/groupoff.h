#ifndef __GROUPOFF_H__
#define __GROUPOFF_H__

#include "MPRNG.h"
#include "printer.h"
#include "watcard.h"

extern MPRNG mprng;

_Task Groupoff {
	Printer &printer;
	unsigned int numStudents;
	unsigned int sodaCost;
	unsigned int groupoffDelay;
	WATCard::FWATCard *futures;
	bool *used;
	unsigned int numGiven;

    void main();
  public:
    Groupoff( Printer &prt, unsigned int numStudents, unsigned int sodaCost, unsigned int groupoffDelay );
    ~Groupoff();
    WATCard::FWATCard giftCard();
};

#endif
