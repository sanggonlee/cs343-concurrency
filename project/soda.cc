#include <iostream>
#include "MPRNG.h"
#include "config.h"
#include "printer.h"
#include "bank.h"
#include "parent.h"
#include "watcardoffice.h"
#include "groupoff.h"
#include "nameserver.h"
#include "vendingmachine.h"
#include "bottlingplant.h"
#include "student.h"
using namespace std;

MPRNG mprng;

void uMain::main() {
	const char *configFile = "soda.config";
	unsigned int seed = getpid();
	switch (argc) {
		case 3:
			seed = atoi(argv[2]);
			if (seed <= 0) {
				cerr << "Error: Seed must be greater than 0" << endl;
				exit(EXIT_FAILURE);
			}
		case 2:
			configFile = argv[1];
		case 1:
			break;
		default:
			cerr << "Usage: soda [ config-file [ Seed ] ]" << endl;
	}
	// Get the config parameters from file
	ConfigParms params;
	processConfigFile(configFile, params);

	mprng.seed(seed);

	// Start tasks
	Printer printer(params.numStudents, params.numVendingMachines, params.numCouriers);
	Bank bank(params.numStudents);
	Parent parent(printer, bank, params.numStudents, params.parentalDelay);
	WATCardOffice watcardOffice(printer, bank, params.numCouriers);
	Groupoff groupoff(printer, params.numStudents, params.sodaCost, params.groupoffDelay);
	NameServer nameServer(printer, params.numVendingMachines, params.numStudents);
	
	VendingMachine *vendingMachines[params.numVendingMachines];
	for (unsigned int v=0; v<params.numVendingMachines; v++) {
		vendingMachines[v] = new VendingMachine(printer, nameServer, v, params.sodaCost, params.maxStockPerFlavour);
	}
	
	BottlingPlant *bottlingPlant = new BottlingPlant(printer, nameServer, params.numVendingMachines, params.maxShippedPerFlavour, params.maxStockPerFlavour, params.timeBetweenShipments);
	
	Student *students[params.numStudents];
	for (unsigned int s=0; s<params.numStudents; s++) {
		students[s] = new Student(printer, nameServer, watcardOffice, groupoff, s, params.maxPurchases);
	}
	
	// Wait for students to finish
	for (unsigned int s=0; s<params.numStudents; s++) {
		delete students[s];
	}

	// Finish BottlingPlant before finishing VendingMachine's to prevent deadlock
	delete bottlingPlant;
	for (unsigned int v=0; v<params.numVendingMachines; v++) {
		delete vendingMachines[v];
	}
}
