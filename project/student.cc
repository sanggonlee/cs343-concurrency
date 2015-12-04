#include "student.h"
#include "watcard.h"
#include "vendingmachine.h"

/*
 *	Main routine for Student
 */
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
			// Wait for gift card or Watcard to be ready
			_Select(giftCard) {
				WATCard *card = giftCard;
				for ( ;; ) {	// Looping until purchase successful
					try {
						vendingMachine->buy(flavour, *card);
						
						// Buying with gift card successful, reset the future to prevent further usage
						printer.print(Printer::Kind::Student, id, 'G', (int)card->getBalance());
						giftCard.reset();
						delete card;
						break TryBuying;	// attempt next purchase
					} catch (VendingMachine::Stock e) {
						// Not enough stock, change vending machine and attempt to buy again
						vendingMachine = nameServer.getMachine(id);
						printer.print(Printer::Kind::Student, id, 'V', (int)vendingMachine->getId());
					}
					// Funds exception need not be handled for gift card because
					// every gift card comes with exactly soda cost amount in it
				}
			} or _Select(watCard) {
				TryWatcard: for ( ;; ) {
					try {
						realWatCard = watCard;
					} catch (WATCardOffice::Lost e) {
						// If card lost, delete this card and request to create a new card
						printer.print(Printer::Kind::Student, id, 'L');
						delete realWatCard;
						watCard = cardOffice.create(id, 5);
						break TryWatcard;	// Wait for a new card to arrive instead of trying next purchase
					}
					
					// didn't lose the card
					try {
						vendingMachine->buy(flavour, *realWatCard);
						
						// Buying with Watcard successful
						printer.print(Printer::Kind::Student, id, 'B', (int)realWatCard->getBalance());
						break TryBuying;	// attempt next purchase
					} catch (VendingMachine::Stock e) {
						// Not enough stock, change vending machine and attempt to buy again
						vendingMachine = nameServer.getMachine(id);
						printer.print(Printer::Kind::Student, id, 'V', (int)vendingMachine->getId());
					} catch (VendingMachine::Funds e) {
						// Not enough funds, request more money to be transferred and wait for transfer to complete
						watCard = cardOffice.transfer(id, vendingMachine->cost()+5, realWatCard);
						break TryWatcard;	// Wait for a new card to arrive instead of trying next purchase
					}
				}
			}
		}
	}
	
	// This is needed to resolve memory leak when the last purchase was
	// made with gift card but Watcard was made ready meantime
	if (watCard.available()) {
		WATCard* garb = watCard();
		delete garb;
	}
	printer.print(Printer::Kind::Student, id, 'F');
	// Only accept destructor when all the purchases are complete
	_Accept(~Student);
}

/*
 *	Constructor for Student
 */
Student::Student( Printer &prt, NameServer &nameServer, WATCardOffice &cardOffice, Groupoff &groupoff, unsigned int id, unsigned int maxPurchases ) 
: printer(prt)
, nameServer(nameServer)
, cardOffice(cardOffice)
, groupoff(groupoff)
, id(id)
, maxPurchases(maxPurchases) {	
}
