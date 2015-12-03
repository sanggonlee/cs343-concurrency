#ifndef __TRUCK_H__
#define __TRUCK_H__

#include "printer.h"
#include "nameserver.h"
#include "bottlingplant.h"
#include "vendingmachine.h"

_Task Truck {
	Printer &printer;
	NameServer &nameServer;
	BottlingPlant &plant;
	unsigned int numVendingMachines;
	unsigned int maxStockPerFlavour;
	VendingMachine **machines;
	unsigned int *cargo;
	unsigned int nextMachine;

	bool isCargoEmpty();
	void main();
  public:
    Truck( Printer &prt, NameServer &nameServer, BottlingPlant &plant, unsigned int numVendingMachines, unsigned int maxStockPerFlavour );
    ~Truck();
};

#endif
