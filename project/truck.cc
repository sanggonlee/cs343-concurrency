#include "truck.h"
#include "vendingmachine.h"

#include <iostream>
using namespace std;

bool Truck::isCargoEmpty() {
	for (unsigned int f=0; f<(unsigned int)VendingMachine::Flavours::NumFlavours; f++) {
		if (cargo[f] != 0) return false;
	}
	return true;
}

void Truck::main() {
	printer.print(Printer::Kind::Truck, 'S');
	VendingMachine **vendingMachines = nameServer.getMachineList();
	for (unsigned int i=0; i<numVendingMachines; i++) {
		machines[i] = vendingMachines[i];
	}

	for ( ;; ) {
		yield(mprng(1, 10));
		unsigned int total = 0;
		try {
			plant.getShipment(cargo);
			for (unsigned int f=0; f<(unsigned int)VendingMachine::Flavours::NumFlavours; f++) {
				total += cargo[f];
			}
			printer.print(Printer::Kind::Truck, 'P', total);
		} catch (BottlingPlant::Shutdown e) {
			break;
		}

		for (unsigned int j=0; j<numVendingMachines; j++) {
			if (isCargoEmpty()) {
				break;
			}

			unsigned int *inven = machines[nextMachine]->inventory();
			printer.print(Printer::Kind::Truck, 'd', nextMachine, total);
			unsigned int notFilled = 0;
			for (unsigned int i=0; i<(unsigned int)VendingMachine::Flavours::NumFlavours; i++) {
				unsigned int toStock = maxStockPerFlavour - inven[i];
				if (toStock > maxStockPerFlavour) {
					// inventory has more than maxStockPerFlavour
					toStock = 0;
				}
				if (toStock > cargo[i]) {
					notFilled += (toStock - cargo[i]);
					toStock = cargo[i];
				}
				inven[i] += toStock;
				cargo[i] -= toStock;
				total -= toStock;
			}
			printer.print(Printer::Kind::Truck, 'U', nextMachine, notFilled);
			printer.print(Printer::Kind::Truck, 'D', nextMachine, total);
			machines[nextMachine]->restocked();
			nextMachine = (nextMachine+1) % numVendingMachines;
		}
	}
	printer.print(Printer::Kind::Truck, 'F');
}

Truck::Truck(Printer &prt, NameServer &nameServer, BottlingPlant &plant, unsigned int numVendingMachines, unsigned int maxStockPerFlavour)
: printer(prt)
, nameServer(nameServer)
, plant(plant)
, numVendingMachines(numVendingMachines)
, maxStockPerFlavour(maxStockPerFlavour)
, nextMachine(0) {
	machines = new VendingMachine*[numVendingMachines];
	cargo = new unsigned int[(unsigned int)VendingMachine::Flavours::NumFlavours];
}

Truck::~Truck() {
	delete[] machines;
	delete[] cargo;
}
