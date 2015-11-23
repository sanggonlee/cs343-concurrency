#include "q2printer.h"
#include <iostream>
using namespace std;

Printer::Printer( const unsigned int NoOfPlayers, const unsigned int NoOfCards ) {
	numPlayers = NoOfPlayers;
	numCards = NoOfCards;
	Printer::buffer = new char[8*numPlayers];
	resetBuffer();
	
	// print the first line
	cout << "Players: " << NoOfPlayers << "    Cards: " << NoOfCards << endl;
	for (unsigned int pi=0; pi<NoOfPlayers; pi++) {
		cout << "P" << pi << '\t';
	}
	cout << endl;
}

Printer::~Printer() {
	delete[] Printer::buffer;
}

void Printer::resetBuffer() {
	for (unsigned int i=0; i<8*numPlayers; i++) {
		if (i%8 == 0) {
			Printer::buffer[i] = '\t';
		} else {
			Printer::buffer[i] = (char)NULL;
		}
	}
}

// Encode multi digit number into buffer starting at index, up to 3 digits
// returns the index ended
unsigned int Printer::encodeNumberToBuffer(unsigned int index, int num) {
	int offset = 0;
	if (num > 99) {
		buffer[index] = num/100 + 48;
		offset += 1;
	}
	if (num > 9) {
		buffer[index+offset] = (num%100)/10 + 48;
		offset += 1;
	}
	buffer[index+offset] = num%10 + 48;
	
	return index+offset;
}

void Printer::printOut(const unsigned int id) {
	for (unsigned int i=0; i<8*numPlayers; i++) {
		if (buffer[i] == (char)NULL) continue;
		
		cout << buffer[i];
	}
	cout << endl;
}

void Printer::prt( 
		const unsigned int id, 
		const unsigned int took, 
		unsigned int PlayersRemaining ) {
	if (buffer[8*id] != '\t') {
		// print out buffer and flush
		for (int i=0; i<8; i++) {
			if (buffer[8*id+i] == '\t') {
				buffer[8*id+i] = '*';
				if (i != 7) buffer[8*id+i+1] = '\t';
				break;
			}
		}
		printOut(id);
		resetBuffer();
	}
	
	int remaining = Printer::numCards < took ? 0 : Printer::numCards - took;
	
	if (PlayersRemaining == 1) {
		buffer[8*id] = '#';
		unsigned int off = encodeNumberToBuffer(8*id+1, Printer::numCards);
		buffer[off+1] = '#';
		if (Printer::numCards % 5 == 0) {
			buffer[off+2] = 'X';
			off += 1;
		}
		buffer[off+2] = '\t';
		printOut(id);
		return;
	}
	
	buffer[8*id] = took < Printer::numCards ? took + 48 : Printer::numCards + 48;
	buffer[8*id+1] = ':';
	
	unsigned int ind = encodeNumberToBuffer(8*id+2, remaining);
	
	if (remaining == 0) {
		buffer[ind+1] = '#';
		if (Printer::numCards % 5 == 0) {
			buffer[ind+2] = 'X';
			ind += 1;
		}
		buffer[ind+2] = '\t';
		printOut(id);
		return;
	}
	
	if (remaining%2 == 0) {
		buffer[ind+1] = '>';
	} else {
		buffer[ind+1] = '<';
	}
	
	if (Printer::numCards % 5 == 0) {
		buffer[ind+2] = 'X';
		ind += 1;
	}
	buffer[ind+2] = '\t';
	
	numCards = remaining;
}
