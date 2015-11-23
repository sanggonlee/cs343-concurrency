#include "q3tallyVotes.h"
#include "q3printer.h"

#include <iostream>
using namespace std;

TallyVotes::Tour TallyVotes::vote( unsigned int id, Tour ballot ) {
	curId = id;
	curBallot = ballot;
	
	return ballot;	// not used
}

void TallyVotes::main() {
	for ( ;; ) {
		unsigned int group[groupSize];
		unsigned int vIndex;
		for (vIndex = 0; vIndex<groupSize; vIndex+=1) {
			_Accept(~TallyVotes) {
				return;
			} or _Accept(vote) {
				group[vIndex] = curId;
				printer.print(group[vIndex], Voter::States::Vote, curBallot);
				switch (curBallot) {
					case Picture:
						picture += 1;
						break;
					case Statue:
						statue += 1;
				}
				numWaiting += 1;
				printer.print(group[vIndex], Voter::States::Block, numWaiting);
		
				if (numWaiting == groupSize) {
					printer.print(group[vIndex], Voter::States::Complete);
				}
			}
		}
		for (vIndex = 0; vIndex<groupSize; vIndex+=1) {
			numWaiting -= 1;
			printer.print(group[vIndex], Voter::States::Unblock, numWaiting);
		
			Tour result = picture > statue ? Picture : Statue;
			printer.print(group[vIndex], Voter::States::Finished, result);
	
			if (numWaiting == 0) {
				picture = 0;
				statue = 0;
			}
		}
	}
}


TallyVotes::TallyVotes( unsigned int group, Printer &printer )
: curId(0)
, printer(printer)
, groupSize(group)
, numWaiting(0)
, picture(0)
, statue(0)
, curBallot(Picture) {
}
