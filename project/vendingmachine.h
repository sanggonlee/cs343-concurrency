#ifndef __VENDING_MACHINE_H__
#define __VENDING_MACHINE_H__

#include <uSemaphore.h>
#include "nameserver.h"
#include "watcard.h"

_Task VendingMachine {
	Printer &printer;
	NameServer &nameServer;
	unsigned int id;
	unsigned int sodaCost;
	unsigned int maxStockPerFlavour;
	unsigned int *stocks;

    void main();
  public:
    enum Flavours { Blues, Classic, Rock, Jazz, NumFlavours };  // flavours of soda
    _Event Funds {};                       // insufficient funds
    _Event Stock {};                       // out of stock for particular flavour
    VendingMachine( Printer &prt, NameServer &nameServer, unsigned int id, unsigned int sodaCost, unsigned int maxStockPerFlavour );
    ~VendingMachine();
    void buy( Flavours flavour, WATCard &card );
    unsigned int *inventory();
    void restocked();
    _Nomutex unsigned int cost();
    _Nomutex unsigned int getId();
};

#endif
