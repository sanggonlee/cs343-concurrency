#include <iostream>
#include "printer.h"
using namespace std;

/*
 * Fills the buffer with NULL's except for tabs at every 
 * first char of 8 characters, in order to fill empty
 * slots unless no other non-empty slots follow after.
 */
void Printer::resetBuffer() {
	for (unsigned int i=0; i<8*numEntities; i++) {
		if (i%8 == 0) {
			buffer[i] = '\t';
		} else {
			buffer[i] = (char)NULL;
		}
	}
}

/*
 * Converts an integer value of int type to an integer
 * represented as an int type
 */
char Printer::intToChar(unsigned int n) {
	return (char)(n+48);
}

/*
 * Prints out buffer
 * Phase 1 (cleanup): starting from the back of the array, replace 
 *   the dummy tabs with NULL until seeing non-whitespace character
 * Phase 2 (print): print out every character in the buffer one by
 *   one
 */
void Printer::printOut() {
	unsigned int i;
	// remove tabs working backwards until not whitespace char
	for (i=8*numEntities-1; i>0; i-=1) {
		if (buffer[i] == '\t') {
			buffer[i] = (char)NULL;
		} else if (buffer[i] != (char)NULL) {
			break;
		}
	}
	// print buffer
	for (i=0; i<8*numEntities; i+=1) {
		if (buffer[i] == (char)NULL) {
			continue;
		} else {
			cout << buffer[i];
		}
	}
	cout << endl;
}

/*
 * Get the starting buffer location for the kind given
 * (Overloaded) Used only for entities whose IDs are not applicable
 */
unsigned int Printer::getBufferLocation(Kind kind) {
	return 8*(unsigned int)kind;
}

/*
 * Get the starting buffer location for the kind and id given
 * (Overloaded) Used only for entities who have IDs
 */
unsigned int Printer::getBufferLocation(Kind kind, unsigned int id) {
	switch (kind) {
		case Student:
			return 8*((unsigned int)kind + id);
		case Vending:
			return 8*((unsigned int)kind + numStudents-1 + id);
		case Courier:
			return 8*((unsigned int)kind + numStudents-1 + numVendingMachines-1 + id);
		default:
			cerr << "Warning: Expecting Student|Vending|Courier but got something else! Investigate!";
			return 0;
	}
}

/*
 * Check if the current buffer should be printed out and flushed
 * Does nothing if it doesn't need to be printed out
 */
void Printer::checkAndPrint(unsigned int location) {
	if (buffer[location] != '\t') {
		printOut();
		resetBuffer();
	}
}

/*
 * Fill other entities' buffer by ... for printing F
 */
void Printer::fillFinished() {
	resetBuffer();
	for (unsigned int i=0; i<numEntities; i++) {
		buffer[8*i] = '.';
		buffer[8*i+1] = '.';
		buffer[8*i+2] = '.';
		buffer[8*i+3] = '\t';
	}
}

/*
 * Constructor for Printer class
 * Also prints out the first 2 header lines of output
 */
Printer::Printer(unsigned int numStudents, unsigned int numVendingMachines, unsigned int numCouriers)
: numStudents(numStudents)
, numVendingMachines(numVendingMachines)
, numCouriers(numCouriers)
, numEntities(6+numStudents+numVendingMachines+numCouriers) {
	buffer = new char[8*numEntities];
	
	resetBuffer();
	
	unsigned int i = 0;
	buffer[i++] = 'P';
	buffer[i++] = 'a';
	buffer[i++] = 'r';
	buffer[i++] = 'e';
	buffer[i++] = 'n';
	buffer[i++] = 't';
	buffer[i++] = '\t';
	buffer[i++] = (char)NULL;
	
	buffer[i++] = 'G';
	buffer[i++] = 'r';
	buffer[i++] = 'o';
	buffer[i++] = 'p';
	buffer[i++] = 'o';
	buffer[i++] = 'f';
	buffer[i++] = 'f';
	buffer[i++] = '\t';
	
	buffer[i++] = 'W';
	buffer[i++] = 'A';
	buffer[i++] = 'T';
	buffer[i++] = 'O';
	buffer[i++] = 'f';
	buffer[i++] = 'f';
	buffer[i++] = '\t';
	buffer[i++] = (char)NULL;
	
	buffer[i++] = 'N';
	buffer[i++] = 'a';
	buffer[i++] = 'm';
	buffer[i++] = 'e';
	buffer[i++] = 's';
	buffer[i++] = '\t';
	buffer[i++] = (char)NULL;
	buffer[i++] = (char)NULL;
	
	buffer[i++] = 'T';
	buffer[i++] = 'r';
	buffer[i++] = 'u';
	buffer[i++] = 'c';
	buffer[i++] = 'k';
	buffer[i++] = '\t';
	buffer[i++] = (char)NULL;
	buffer[i++] = (char)NULL;
	
	buffer[i++] = 'P';
	buffer[i++] = 'l';
	buffer[i++] = 'a';
	buffer[i++] = 'n';
	buffer[i++] = 't';
	buffer[i++] = '\t';
	buffer[i++] = (char)NULL;
	buffer[i++] = (char)NULL;
	
	for (unsigned int j=0; j<numStudents; j++) {
		buffer[i+8*j] = 'S';
		buffer[i+8*j+1] = 't';
		buffer[i+8*j+2] = 'u';
		buffer[i+8*j+3] = 'd';
		if (numStudents > 9) {
			buffer[i+8*j+4] = intToChar(j/10);
			buffer[i+8*j+5] = intToChar(j%10);
			buffer[i+8*j+6] = '\t';
		} else {
			buffer[i+8*j+4] = intToChar(j);
			buffer[i+8*j+5] = '\t';
		}
	}
	i += 8*numStudents;
	for (unsigned int j=0; j<numVendingMachines; j++) {
		buffer[i+8*j] = 'M';
		buffer[i+8*j+1] = 'a';
		buffer[i+8*j+2] = 'c';
		buffer[i+8*j+3] = 'h';
		if (numVendingMachines > 9) {
			buffer[i+8*j+4] = intToChar(j/10);
			buffer[i+8*j+5] = intToChar(j%10);
			buffer[i+8*j+6] = '\t';
		} else {
			buffer[i+8*j+4] = intToChar(j);
			buffer[i+8*j+5] = '\t';
		}
	}
	i += 8*numVendingMachines;
	for (unsigned int j=0; j<numCouriers; j++) {
		buffer[i+8*j] = 'C';
		buffer[i+8*j+1] = 'o';
		buffer[i+8*j+2] = 'u';
		buffer[i+8*j+3] = 'r';
		if (numVendingMachines > 9) {
			buffer[i+8*j+4] = intToChar(j/10);
			buffer[i+8*j+5] = intToChar(j%10);
			buffer[i+8*j+6] = '\t';
		} else {
			buffer[i+8*j+4] = intToChar(j);
			buffer[i+8*j+5] = '\t';
		}
	}
	printOut();
	resetBuffer();
	
	for (i=0; i<8*numEntities; i++) {
		if (i%8 == 7) {
			buffer[i] = '\t';
		} else {
			buffer[i] = '*';
		}
	}
	printOut();
	resetBuffer();
}

/*
 * Destructor for Printer class
 * Also prints the footer line of output
 */
Printer::~Printer() {
	printOut();
	resetBuffer();
	
	for (unsigned int i=0; i<23; i++) {
		buffer[i] = '*';
	}
	printOut();
	
	delete[] buffer;
}


/********************************************************************
  *	For the below 3 methods it is assumed that kind is either Parent,
  * Groupoff, WATCardOffice, NameServer, Truck, or BottlingPlant.
  *******************************************************************/

/*
 * Prints only the state
 */
void Printer::print(Kind kind, char state) {
	unsigned int location = getBufferLocation(kind);

	if (state == 'F') {
		printOut();
		fillFinished();
		buffer[location+2] = (char)NULL;
		buffer[location+3] = (char)NULL;
	} else {
		checkAndPrint(location);
	}
	buffer[location] = state;
	buffer[location+1] = '\t';

	printOut();
	resetBuffer();
}

/*
 * Prints the state and one value
 */
void Printer::print(Kind kind, char state, int value1) {
	unsigned int location = getBufferLocation(kind);
	checkAndPrint(location);
	
	buffer[location] = state;
	if (value1 > 9) {
		buffer[location+1] = intToChar(value1/10);
		buffer[location+2] = intToChar(value1%10);
		buffer[location+3] = '\t';
	} else {
		buffer[location+1] = intToChar(value1);
		buffer[location+2] = '\t';
	}

	printOut();
	resetBuffer();
}

/*
 * Prints the state and two values
 */
void Printer::print(Kind kind, char state, int value1, int value2) {
	unsigned int location = getBufferLocation(kind);
	checkAndPrint(location);
	
	buffer[location] = state;
	if (value1 > 9) {
		buffer[location+1] = intToChar(value1/10);
		location++;
	}
	buffer[location+1] = intToChar(value1%10);
	buffer[location+2] = ',';
	if (value2 > 9) {
		buffer[location+3] = intToChar(value2/10);
		location++;
	}
	buffer[location+3] = intToChar(value2%10);
	buffer[location+4] = '\t';

	printOut();
	resetBuffer();
}

/*******************************************************************
 * For the below 3 methods it is assumed that kind is either Student,
 * Vending, or Courier
 *******************************************************************/

/*
 * Prints only the state
 */
void Printer::print(Kind kind, unsigned int lid, char state) {
	unsigned int location = getBufferLocation(kind, lid);
	
	if (state == 'F') {
		printOut();
		fillFinished();
		buffer[location+2] = (char)NULL;
		buffer[location+3] = (char)NULL;
	} else {
		checkAndPrint(location);
	}
	buffer[location] = state;
	buffer[location+1] = '\t';

	printOut();
	resetBuffer();
}

/*
 * Prints the state and one value
 */
void Printer::print(Kind kind, unsigned int lid, char state, int value1) {
	unsigned int location = getBufferLocation(kind, lid);
	checkAndPrint(location);
	
	buffer[location] = state;
	if (value1 > 9) {
		buffer[location+1] = intToChar(value1/10);
		buffer[location+2] = intToChar(value1%10);
		buffer[location+3] = '\t';
	} else {
		buffer[location+1] = intToChar(value1);
		buffer[location+2] = '\t';
	}

	printOut();
	resetBuffer();
}

/*
 * Prints the state and two values
 */
void Printer::print(Kind kind, unsigned int lid, char state, int value1, int value2) {
	unsigned int location = getBufferLocation(kind, lid);
	checkAndPrint(location);
	
	buffer[location] = state;
	if (value1 > 9) {
		buffer[location+1] = intToChar(value1/10);
		location++;
	}
	buffer[location+1] = intToChar(value1%10);
	buffer[location+2] = ',';
	if (value2 > 9) {
		buffer[location+3] = intToChar(value2/10);
		location++;
	}
	buffer[location+3] = intToChar(value2%10);
	buffer[location+4] = '\t';

	printOut();
	resetBuffer();
}
