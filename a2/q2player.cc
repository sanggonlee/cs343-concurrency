#include "q2player.h"
#include <iostream>
using namespace std;


unsigned int Player::numRemainingPlayers = 0;
unsigned int Player::numRemainingCards = 0;

/**
 *	Player routines
 */
 
void Player::players( unsigned int num ) {
	Player::numRemainingPlayers = num;
}

Player::Player( Printer &printer, unsigned int id ) {
	Player::id = id;
	Player::printer = &printer;
}

void Player::start( Player &lp, Player &rp ) {
	left = &lp;
	right = &rp;
	resume();
}

void Player::play( unsigned int deck ) {
	Player::numRemainingCards = deck;
	resume();
}

void Player::main() {
	suspend();	// wait for play after start is called
	
	for ( ;; ) {
		if (Player::numRemainingPlayers <= 1) {
			// pass dummy variable "took", it won't be used
			printer->prt(id, 0, Player::numRemainingPlayers);
			// game ends
			return;
		}
		
		unsigned int take = prng(1, 8);
		printer->prt(id, take, Player::numRemainingPlayers);
		
		if (take >= Player::numRemainingCards) {
			// another ending scenario
			return;
		}

		if ((Player::numRemainingCards%5) == 0) {
			// dies
			Player::numRemainingPlayers -= 1;
			left->right = right;
			right->left = left;
		}

		Player::numRemainingCards -= take;
		
		if ((Player::numRemainingCards)%2 == 1) {	// odd
			left->play(Player::numRemainingCards);
		} else {					// even
			right->play(Player::numRemainingCards);
		}
	}
}
