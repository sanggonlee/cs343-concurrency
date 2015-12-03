#ifndef __BOTTLING_PLANT_H__
#define __BOTTLING_PLANT_H__

#include "MPRNG.h"
#include "printer.h"
#include "nameserver.h"

extern MPRNG mprng;

_Task BottlingPlant {
	Printer &printer;
	NameServer &nameServer;
	unsigned int numVendingMachines;
	unsigned int maxShippedPerFlavour;
	unsigned int maxStockPerFlavour;
	unsigned int timeBetweenShipments;
	unsigned int *currProduction;
	bool shuttingDown;

    void main();
  public:
    _Event Shutdown {};                    // shutdown plant
    BottlingPlant( Printer &prt, NameServer &nameServer, unsigned int numVendingMachines, unsigned int maxShippedPerFlavour, unsigned int maxStockPerFlavour, unsigned int timeBetweenShipments );
    ~BottlingPlant();
    void getShipment( unsigned int cargo[] );
};

#endif
