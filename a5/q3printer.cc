#include "q3printer.h"
#include <iostream>

using namespace std;

/*
 * Fills the buffer with NULL's except for tabs at every 
 * first char of 8 characters, in order to fill empty
 * slots unless no other non-empty slots follow after.
 */
void Printer::resetBuffer() {
	for (unsigned int i=0; i<8*numVoters; i+=1) {
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
	for (i=8*numVoters-1; i>=0; i-=1) {
		if (buffer[i] == '\t') {
			buffer[i] = (char)NULL;
		} else if (buffer[i] != (char)NULL) {
			break;
		}
	}
	// print buffer
	for (i=0; i<8*numVoters; i+=1) {
		if (buffer[i] == (char)NULL) {
			continue;
		} else {
			cout << buffer[i];
		}
	}
	cout << endl;
}

/*
 * The constructor also prints the first two lines of
 * output, which are expected to be printed in any case
 */
Printer::Printer( unsigned int voters )
: numVoters(voters) {
	buffer = new char[8*voters];
	
	resetBuffer();
	unsigned int i;
	// Print voter names first
	for (i=0; i<voters; i+=1) {
		buffer[8*i+0] = 'V';
		buffer[8*i+1] = 'o';
		buffer[8*i+2] = 't';
		buffer[8*i+3] = 'e';
		buffer[8*i+4] = 'r';
		if (i > 9) {
			buffer[8*i+5] = intToChar(i/10);
			buffer[8*i+6] = intToChar(i%10);
			buffer[8*i+7] = '\t';
		} else {
			buffer[8*i+5] = intToChar(i);
			buffer[8*i+6] = '\t';
		}
	}
	printOut();
	
	resetBuffer();
	// Print the separator line
	for (i=0; i<8*voters; i+=1) {
		if (i%8 == 7) {	// last
			buffer[i] = '\t';
		} else {
			buffer[i] = '=';
		}
	}
}

/*
 * The destructor also prints out the ending separator
 * line and a message which should be printed in any case
 */
Printer::~Printer() {
	delete[] buffer;
	cout << "=================" << endl
		 << "All tours started" << endl;
}

/*
 * Inserts to the buffer for when only the state should be printed
 */
void Printer::print( unsigned int id, Voter::States state ) {
	// If there's already a buffer at this slot, print out and flush
	if (buffer[8*id] != '\t') {
		printOut();
		resetBuffer();
	}
	switch (state) {
		case Voter::States::Start:
			buffer[8*id] = 'S';
			break;
		case Voter::States::Complete:
			buffer[8*id] = 'C';
			break;
		case Voter::States::Barging:
			buffer[8*id] = 'b';
			break;
		default:
			cerr<< "print of type 1 got something else than S or C. Abort" <<endl;
			exit(EXIT_FAILURE);
	}
	buffer[8*id+1] = '\t';
}

/*
 * Insert to the buffer for when a Tour type should be printed
 */
void Printer::print( unsigned int id, Voter::States state, TallyVotes::Tour vote ) {
	// If there's already a buffer at this slot, print out and flush
	if (buffer[8*id] != '\t') {
		printOut();
		resetBuffer();
	}
	switch (state) {
		case Voter::States::Vote:
			buffer[8*id] = 'V';
			buffer[8*id+1] = ' ';
			if (vote == TallyVotes::Tour::Picture) {
				buffer[8*id+2] = 'p';
			} else {
				buffer[8*id+2] = 's';
			}
			buffer[8*id+3] = '\t';
			break;
		case Voter::States::Finished:
			for (unsigned int i=0; i<numVoters; i+=1) {
				buffer[8*i] = '.';
				buffer[8*i+1] = '.';
				buffer[8*i+2] = '.';
				buffer[8*i+3] = '\t';
			}
			buffer[8*id] = 'F';
			buffer[8*id+1] = ' ';
			if (vote == TallyVotes::Tour::Picture) {
				buffer[8*id+2] = 'p';
			} else if (vote == TallyVotes::Tour::Statue) {
				buffer[8*id+2] = 's';
			} else {
				cerr << "Result must be Picture or Statue but got something else. Abort." << endl;
				exit(-1);
			}
			printOut();
			resetBuffer();
			break;
		default:
			cerr<< "print of type 2 got something else than V or F. Abort" <<endl;
			exit(EXIT_FAILURE);
	}
}

/*
 * Insert to the buffer for when the number of blocking threads needs
 * to be printed
 */
void Printer::print( unsigned int id, Voter::States state, unsigned int numBlocked ) {
	// If there's already a buffer at this slot, print out and flush
	if (buffer[8*id] != '\t') {
		printOut();
		resetBuffer();
	}
	switch (state) {
		case Voter::States::Block:
			buffer[8*id] = 'B';
			break;
		case Voter::States::Unblock:
			buffer[8*id] = 'U';
			break;
		default:
			cerr<< "print of type 1 got something else than B or U. Abort" <<endl;
			exit(EXIT_FAILURE);
	}
	buffer[8*id+1] = ' ';
	buffer[8*id+2] = intToChar((int)numBlocked);
	buffer[8*id+3] = '\t';
}
