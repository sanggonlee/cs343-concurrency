#include "student.h"
#include "watcard.h"
#include "vendingmachine.h"

#include <iostream>
using namespace std;

void Student::main() {
	unsigned int numBottlesToBuy = mprng(1, maxPurchases);
	VendingMachine::Flavours flavour = (VendingMachine::Flavours)mprng(3);
	printer.print(Printer::Kind::Student, id, 'S', (int)flavour, (int)numBottlesToBuy);

	WATCard::FWATCard watCard = cardOffice.create(id, 5);
	
	WATCard::FWATCard giftCard = groupoff.giftCard();
	
	VendingMachine *vendingMachine = nameServer.getMachine(id);
	printer.print(Printer::Kind::Student, id, 'V', (int)vendingMachine->getId());
	
	WATCard *realWatCard = NULL;	// suppress warning
	
	for (unsigned int attempt=0; attempt<numBottlesToBuy; attempt++) {
		yield(mprng(1,10));
		TryBuying: for ( ;; ) {
			_Select(giftCard) {
				WATCard *card = giftCard;
				for ( ;; ) {
					try {
						vendingMachine->buy(flavour, *card);
						printer.print(Printer::Kind::Student, id, 'G', (int)card->getBalance());
						giftCard.reset();
						delete card;
						break TryBuying;
					} catch (VendingMachine::Stock e) {
						vendingMachine = nameServer.getMachine(id);
						printer.print(Printer::Kind::Student, id, 'V', (int)vendingMachine->getId());
					}
				}
			} or _Select(watCard) {
				TryWatcard: for ( ;; ) {
					try {
						realWatCard = watCard;
					} catch (WATCardOffice::Lost e) {
						printer.print(Printer::Kind::Student, id, 'L');
						delete realWatCard;
						watCard = cardOffice.create(id, 5);
						break TryWatcard;
					}
					
					// didn't lose the card
					try {
						vendingMachine->buy(flavour, *realWatCard);
						printer.print(Printer::Kind::Student, id, 'B', (int)realWatCard->getBalance());
						break TryBuying;
					} catch (VendingMachine::Stock e) {
						vendingMachine = nameServer.getMachine(id);
						printer.print(Printer::Kind::Student, id, 'V', (int)vendingMachine->getId());
					} catch (VendingMachine::Funds e) {
						watCard = cardOffice.transfer(id, vendingMachine->cost()+5, realWatCard);
						break TryWatcard;
					}
				}
			}
		}
	}
	
	if (watCard.available()) {
		WATCard* garb = watCard();
		delete garb;
	}
	printer.print(Printer::Kind::Student, id, 'F');
	_Accept(~Student);
}

Student::Student( Printer &prt, NameServer &nameServer, WATCardOffice &cardOffice, Groupoff &groupoff, unsigned int id, unsigned int maxPurchases ) 
: printer(prt)
, nameServer(nameServer)
, cardOffice(cardOffice)
, groupoff(groupoff)
, id(id)
, maxPurchases(maxPurchases) {	
}
