#include "groupoff.h"

/*
 *	Main routine for Groupoff
 */
void Groupoff::main() {
	printer.print(Printer::Kind::Groupoff, 'S');
	// Only assign one gift card to each Student
	for (unsigned int g=0; g<numStudents; g++) {
		_Accept(~Groupoff) {
			break;
		} or _Accept(giftCard) {
			yield(groupoffDelay);
			unsigned int checking;
			// Loop until finding a future gift not used already
			for ( ;; ) {
				checking = mprng(numStudents-1);
				if (!used[checking]) {
					used[checking] = true;
					WATCard *gift = new WATCard();
					gift->deposit(sodaCost);
					printer.print(Printer::Kind::Groupoff, 'D', sodaCost);
					futures[checking].delivery(gift);
					break;
				}
			}
		}
	}
	printer.print(Printer::Kind::Groupoff, 'F');
	_Accept(~Groupoff);
}

/*
 *	Give a new future gift card every time it's called
 */
WATCard::FWATCard Groupoff::giftCard() {
	WATCard::FWATCard ret = futures[numGiven];
	numGiven++;
	return ret;
}

/*
 *	Constructor for Groupoff
 */
Groupoff::Groupoff(Printer &prt, unsigned int numStudents, unsigned int sodaCost, unsigned int groupoffDelay)
: printer(prt)
, numStudents(numStudents)
, sodaCost(sodaCost)
, groupoffDelay(groupoffDelay)
, numGiven(0) {
	futures = new WATCard::FWATCard[numStudents];
	used = new bool[numStudents];
	for (unsigned int i=0; i<numStudents; i++) {
		used[i] = false;
	}
}

/*
 *	Destructor for Groupoff
 */
Groupoff::~Groupoff() {
	for (unsigned int i=0; i<numStudents; i++) {
		if (futures[i].available()) {
			delete futures[i]();
		}
	}
	delete[] futures;
	delete[] used;
}
