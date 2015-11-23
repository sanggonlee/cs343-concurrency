#ifndef __Q2_PRINTER_H__
#define __Q2_PRINTER_H__

#include "q2tallyVotes.h"
#include "q2voter.h"

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
