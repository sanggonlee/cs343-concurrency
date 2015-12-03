#include "parent.h"

#include <iostream>
using namespace std;

void Parent::main() {
	printer.print(Printer::Kind::Parent, 'S');
	for ( int a=0;a<60;a++ ) {
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

Parent::Parent(Printer &prt, Bank &bank, unsigned int numStudents, unsigned int parentalDelay)
: printer(prt)
, bank(bank)
, numStudents(numStudents)
, parentalDelay(parentalDelay) {
}

Parent::~Parent() {
}
