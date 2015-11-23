#ifndef __Q3_PRINTER_H__
#define __Q3_PRINTER_H__

#include "q3tallyVotes.h"
#include "q3voter.h"

_Monitor Printer {
	char *buffer;
	unsigned int numVoters;
	
	void resetBuffer();
	char intToChar(unsigned int n);
	void printOut();
  public:
    Printer( unsigned int voters );
    ~Printer();
    void print( unsigned int id, Voter::States state );
    void print( unsigned int id, Voter::States state, TallyVotes::Tour vote );
    void print( unsigned int id, Voter::States state, unsigned int numBlocked );
};

#endif
