#include "parent.h"

/*
 *	Main routine for Parent
 *	Simply periodically deposit to the bank by random amount
 */
void Parent::main() {
	printer.print(Printer::Kind::Parent, 'S');
	for ( ;; ) {
		_Accept(~Parent) {
			break;
		} _Else {
			yield(parentalDelay);
			unsigned int studentId = mprng(numStudents-1);
			unsigned int amount = mprng(1,3);
			printer.print(Printer::Kind::Parent, 'D', (int)studentId, (int)amount);
			bank.deposit(studentId, amount);
		}
	}
	printer.print(Printer::Kind::Parent, 'F');
}

/*
 *	Constructor for Parent
 */
Parent::Parent(Printer &prt, Bank &bank, unsigned int numStudents, unsigned int parentalDelay)
: printer(prt)
, bank(bank)
, numStudents(numStudents)
, parentalDelay(parentalDelay) {
}

/*
 *	Destructor for Parent
 */
Parent::~Parent() {
}
