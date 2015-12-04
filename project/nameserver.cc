#include "nameserver.h"
#include "vendingmachine.h"

/*
 *	Main routine for NameServer
 */
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

/*
 *	Constructor for NameServer
 */
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

/*
 *	Destructor for NameServer
 */
NameServer::~NameServer() {
	delete[] machines;
	delete[] currStudentPositions;
}

/*
 *	Register a VendingMachine and assign a Student to a VendingMachine,
 *	by first-come-first-serve basis
 */
void NameServer::VMregister(VendingMachine *vendingMachine) {
	machines[currAssignment] = vendingMachine;
	currStudentPositions[currAssignment % numStudents] = currAssignment;
	printer.print(Printer::Kind::NameServer, 'R', vendingMachine->getId());
	currAssignment = (currAssignment+1) % numVendingMachines;
}

/*
 *	Give a VendingMachine to the calling Student
 */
VendingMachine *NameServer::getMachine(unsigned int id) {
	// If the Student's assigned Vending Machine is not registerd, block until it is
	while (currStudentPositions[id] == (unsigned int)(-1)) {
		_Accept(VMregister);
	}
	VendingMachine *machine = machines[currStudentPositions[id]];
	// Assign next VendingMachine to this Student for next time
	currStudentPositions[id] = (currStudentPositions[id]+1) % numVendingMachines;
	printer.print(Printer::Kind::NameServer, 'N', (int)id, (int)currStudentPositions[id]);
	return machine;
}

/*
 *	Give all the machines to Truck
 */
VendingMachine **NameServer::getMachineList() {
	return machines;
}
