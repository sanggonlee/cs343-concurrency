#ifndef __NAMESERVER_H__
#define __NAMESERVER_H__

#include "printer.h"
_Task VendingMachine;

_Task NameServer {
	Printer &printer;
	unsigned int numVendingMachines;
	unsigned int numStudents;
	VendingMachine **machines;
	unsigned int *currStudentPositions;
	unsigned int currAssignment;

    void main();
  public:
    NameServer( Printer &prt, unsigned int numVendingMachines, unsigned int numStudents );
    ~NameServer();
    void VMregister( VendingMachine *vendingmachine );
    VendingMachine *getMachine( unsigned int id );
    VendingMachine **getMachineList();
};

#endif
