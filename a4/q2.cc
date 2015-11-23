#include <iostream>
#include "MPRNG.h"
#include "q2tallyVotes.h"
#include "q2printer.h"
#include "q2voter.h"

using namespace std;

MPRNG mprng;

void uMain::main() {
	int V = 6;
	int G = 3;
	int seed = getpid();
	
	switch (argc) {
		case 4:
			seed = atoi(argv[3]);
			if (seed <= 0) {
				cerr << "ERROR: seed should be a positive integer." << endl;
				exit(EXIT_FAILURE);
			}
		case 3:
			G = atoi(argv[2]);
			if (G <= 0 || G%2 == 0) {
				cerr << "ERROR: G should be a positive odd integer." << endl;
				exit(EXIT_FAILURE);
			}
		case 2:
			V = atoi(argv[1]);
			if (V <= 0) {
				cerr << "ERROR: V should be a positive integer." << endl;
				exit(EXIT_FAILURE);
			} else if (V%G != 0) {
				cerr << "ERROR: V should be divisible by G." << endl;
				exit(EXIT_FAILURE);
			}
		case 1:
			break;
		default:
			cerr << "Usage: vote [ V [ G [ Seed ] ] ]" << endl;
			exit(EXIT_FAILURE);
	}
	
	mprng.seed(seed);
	
	Printer printer(V);
	TallyVotes tallyVotes(G, printer);
	Voter *voters[V];
	for (int vId=0; vId<V; vId+=1) {
		voters[vId] = new Voter(vId, tallyVotes, printer);
	}
	for (int vId=0; vId<V; vId+=1) {
		delete voters[vId];
	}
}
