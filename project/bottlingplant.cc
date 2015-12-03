#include "bottlingplant.h"
#include "vendingmachine.h"
#include "truck.h"

#include <iostream>
using namespace std;

void BottlingPlant::main() {
	printer.print(Printer::Kind::BottlingPlant, 'S');
	Truck *truck = new Truck(printer, nameServer, *this, numVendingMachines, maxStockPerFlavour);
	for ( ;; ) {
		unsigned int generated = 0;
		for (unsigned int i=0; i<(unsigned int)VendingMachine::Flavours::NumFlavours; i++) {
			currProduction[i] = mprng(maxShippedPerFlavour);
			generated += currProduction[i];
		}
		yield(timeBetweenShipments);
		printer.print(Printer::Kind::BottlingPlant, 'G', generated);
		
		_Accept(~BottlingPlant) {
			shuttingDown = true;
			_Accept(getShipment) {
				break;
			}
		} or _Accept(getShipment) {
			printer.print(Printer::Kind::BottlingPlant, 'P');
		}
	}
	delete truck;
	printer.print(Printer::Kind::BottlingPlant, 'F');
}

BottlingPlant::BottlingPlant(Printer &prt, NameServer &nameServer, unsigned int numVendingMachines, unsigned int maxShippedPerFlavour, unsigned int maxStockPerFlavour, unsigned int timeBetweenShipments) 
: printer(prt)
, nameServer(nameServer)
, numVendingMachines(numVendingMachines)
, maxShippedPerFlavour(maxShippedPerFlavour)
, maxStockPerFlavour(maxStockPerFlavour)
, timeBetweenShipments(timeBetweenShipments)
, shuttingDown(false) {
	currProduction = new unsigned int[(unsigned int)VendingMachine::Flavours::NumFlavours];
}

BottlingPlant::~BottlingPlant() {
	delete[] currProduction;
}

void BottlingPlant::getShipment(unsigned int cargo[]) {
	if (shuttingDown) {
		uRendezvousAcceptor();
		_Throw Shutdown();
	}

	// reset cargo, so implicitly emptied
	for (unsigned int i=0; i<(unsigned int)VendingMachine::Flavours::NumFlavours; i++) {
		cargo[i] = currProduction[i];
	}
}
