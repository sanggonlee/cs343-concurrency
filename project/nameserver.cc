#include "nameserver.h"
#include "vendingmachine.h"

void NameServer::main() {
	printer.print(Printer::Kind::NameServer, 'S');
	for (unsigned int i=0; i<numVendingMachines; i++) {
		_Accept(VMregister);
	}
	for ( ;; ) {
		_Accept(~NameServer) {
			printer.print(Printer::Kind::NameServer, 'F');
			break;
		} or _Accept(getMachine) {
		} or _Accept(getMachineList) {
		}
	}
}

NameServer::NameServer(Printer &prt, unsigned int numVendingMachines, unsigned int numStudents)
: printer(prt)
, numVendingMachines(numVendingMachines)
, numStudents(numStudents)
, currAssignment(0) {
	machines = new VendingMachine* [numVendingMachines];
	for (unsigned int m=0; m<numVendingMachines; m++) {
		machines[m] = NULL;
	}
	currStudentPositions = new unsigned int[numStudents];
	for (unsigned int s=0; s<numStudents; s++) {
		currStudentPositions[s] = (unsigned int)(-1);	// default to invalid
	}
}

NameServer::~NameServer() {
	delete[] machines;
	delete[] currStudentPositions;
}

void NameServer::VMregister(VendingMachine *vendingMachine) {
	machines[currAssignment] = vendingMachine;
	currStudentPositions[currAssignment % numStudents] = currAssignment;
	printer.print(Printer::Kind::NameServer, 'R', vendingMachine->getId());
	currAssignment = (currAssignment+1) % numVendingMachines;
}

VendingMachine *NameServer::getMachine(unsigned int id) {
	while (currStudentPositions[id] == (unsigned int)(-1)) {
		_Accept(VMregister);
	}
	VendingMachine *machine = machines[currStudentPositions[id]];
	currStudentPositions[id] = (currStudentPositions[id]+1) % numVendingMachines;
	printer.print(Printer::Kind::NameServer, 'N', (int)id, (int)currStudentPositions[id]);
	return machine;
}

VendingMachine **NameServer::getMachineList() {
	return machines;
}
