#include "vendingmachine.h"
#include "nameserver.h"

/*
 *	Main routine for VendingMachine
 */
void VendingMachine::main() {
	printer.print(Printer::Kind::Vending, id, 'S', sodaCost);
	nameServer.VMregister(this);
	for ( ;; ) {
		_Accept(~VendingMachine) {
			break;
		} or _Accept(inventory) {
			printer.print(Printer::Kind::Vending, id, 'r');
		} or _Accept(restocked) {
			printer.print(Printer::Kind::Vending, id, 'R');
		} or _Accept(buy) {
		}
	}
	printer.print(Printer::Kind::Vending, id, 'F');
}

/*
 *	Constructor for VendingMachine
 */
VendingMachine::VendingMachine(Printer &prt, NameServer &nameServer, unsigned int id, unsigned int sodaCost, unsigned int maxStockPerFlavour)
: printer(prt)
, nameServer(nameServer)
, id(id)
, sodaCost(sodaCost)
, maxStockPerFlavour(maxStockPerFlavour) {
	stocks = new unsigned int[NumFlavours];
	for (unsigned int i=0; i<NumFlavours; i++) {
		stocks[i] = 0;
	}
}

/*
 *	Destructor for VendingMachine
 */
VendingMachine::~VendingMachine() {
	delete[] stocks;
}

/*
 *	Try buying from this machine
 */
void VendingMachine::buy(Flavours flavour, WATCard &card) {
	if (card.getBalance() < sodaCost) {
		uRendezvousAcceptor();	// break rendezvous
		_Throw Funds();
	} else if (stocks[(unsigned int)flavour] == 0) {
		uRendezvousAcceptor();	// break rendezvous
		_Throw Stock();
	} else {
		stocks[(unsigned int)flavour]--;
		card.withdraw(sodaCost);
		printer.print(Printer::Kind::Vending, id, 'B', (int)flavour, stocks[(unsigned int)flavour]);
	}
}

/*
 *	Return inventory
 */
unsigned int *VendingMachine::inventory() {
	return stocks;
}

/*
 *	Get notifed when this machine is restocked
 */
void VendingMachine::restocked() {
}

/*
 *	Return soda cost
 */
unsigned int VendingMachine::cost() {
	return sodaCost;
}

/*
 *	Return this machine's id
 */
unsigned int VendingMachine::getId() {
	return id;
}
