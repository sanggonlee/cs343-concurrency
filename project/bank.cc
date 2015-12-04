#include "bank.h"

/*
 *	Constructor for Bank
 */
Bank::Bank(unsigned int numStudents) {
	balances = new unsigned int[numStudents];
	for (unsigned int i=0; i<numStudents; i++) {
		balances[i] = 0;
	}
}

/*
 *	Destructor for Bank
 */
Bank::~Bank() {
	delete[] balances;
}

/*
 *	Deposit money to bank
 */
void Bank::deposit(unsigned int id, unsigned int amount) {
	balances[id] += amount;
}

/*
 *	Withdraw money from bank
 *	Block if not enough balance and wait for deposit
 */
void Bank::withdraw(unsigned int id, unsigned int amount) {
	for ( ;; ) {
		_When (amount > balances[id]) _Accept(deposit);
		if (amount <= balances[id]) {
			break;
		}
	}
	balances[id] -= amount;
}
