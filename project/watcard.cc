#include "watcard.h"

/*
 *	Constructor for WATCard
 */
WATCard::WATCard()
: balance(0) {
}

/*
 *	Destructor for WATCard
 */
WATCard::~WATCard() {
}

/*
 *	Add amount to balance
 */
void WATCard::deposit(unsigned int amount) {
	balance += amount;
}

/*
 *	Subtract amount from balance
 *	Non-negativity is checked externally, this routine assumes
 *	enough balance given the amount
 */
void WATCard::withdraw(unsigned int amount) {
	balance -= amount;
}

/*
 *	Return balance
 */
unsigned int WATCard::getBalance() {
	return balance;
}
