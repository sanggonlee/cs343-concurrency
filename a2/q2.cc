#include <iostream>
#include <stdlib.h>
#include "PRNG.h"
#include "q2printer.h"
#include "q2player.h"
using namespace std;

PRNG prng;

void uMain::main() {
	int numOfGames = 5;
	int seed = getpid();
	switch (argc) {
		case 3:
			seed = atoi(argv[2]);
			if (seed <= 0) {
				cerr << "Seed should be a positive integer" << endl;
				exit(-1);
			}
		case 2:
			numOfGames = atoi(argv[1]);
			if (numOfGames < 0) {
				cerr << "Seed should be a non-negative integer" << endl;
				exit(-1);
			}
		case 1:
			break;
		default:
			cerr << "Usage: cardgame [ numberOfGames [ Seed ] ]" << endl;
			exit(-1);
	}
	
	prng.seed(seed);
	
	for (int gameIndex=0; gameIndex<numOfGames; gameIndex++) {
		
		const unsigned int numPlayers = prng(2, 10);
		const unsigned int numCards = prng(10, 200);
		int startingPlayerId = prng(numPlayers-1);
		
		Printer printer(numPlayers, numCards);
		
		
		Player* players[numPlayers];
		
		// initialize players
		for (unsigned int pi1=0; pi1<numPlayers; pi1++) {
			players[pi1] = new Player(printer, pi1);
		}
		Player::players(numPlayers);
		
		// call start for each player to register their left & right
		players[0]->start(*players[numPlayers-1], *players[1]);
		for (unsigned int pi2=1; pi2<numPlayers-1; pi2++) {
			players[pi2]->start(*players[pi2-1], *players[pi2+1]);
		}
		players[numPlayers-1]->start(*players[numPlayers-2], *players[0]);
		
		players[startingPlayerId]->play(numCards);
		
		for (unsigned int pi3=0; pi3<numPlayers; pi3++) {
			delete players[pi3];
		}
		
		if (gameIndex != numOfGames-1) {
			cout << endl << endl;
		}
	}
}
