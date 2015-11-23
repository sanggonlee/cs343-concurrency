#include "q3voter.h"
#include "q3printer.h"

/*
 * The main routine for Voter task
 * 1. Yield for a random number of 0-19 time-slices
 * 2. Print Start
 * 3. Compute the vote choice of random value (Picture or Statue) 
 *    for this voter
 * 4. Vote on the vote tallier
 */
void Voter::main() {
	yield(mprng(19));
	printer.print(id, Start);
	// Cast 0 to Picture and 1 to Statue
	TallyVotes::Tour votingFor = (TallyVotes::Tour)mprng(1);
	tallier.vote(id, votingFor);
}

/*
 * Constructor for Voter task
 */
Voter::Voter( unsigned int id, TallyVotes &voteTallier, Printer &printer )
: tallier(voteTallier)
, printer(printer)
, id(id) {
}
