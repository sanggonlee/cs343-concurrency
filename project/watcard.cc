#include "watcard.h"

#include <iostream>
using namespace std;

WATCard::WATCard()
: balance(0) {
}

WATCard::~WATCard() {
}

void WATCard::deposit(unsigned int amount) {
	balance += amount;
}

void WATCard::withdraw(unsigned int amount) {
	balance -= amount;
}

unsigned int WATCard::getBalance() {
	return balance;
}
