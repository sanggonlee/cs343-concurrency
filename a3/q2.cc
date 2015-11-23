#include <iostream>
#include <string>
#include <stdexcept>
#include "MPRNG.h"
#include "q2buffer.h"
#include "q2producer.h"
#include "q2consumer.h"

using namespace std;

MPRNG mPRNG;

void uMain::main() {
	
	int cons = 5;
	int prods = 3;
	int produce = 10;
	int bufferSize = 10;
	int delays = cons+prods;

	switch (argc) {
		case 6:
			try {
				delays = stoi(argv[5]);
			} catch (std::invalid_argument e) {
				cerr << "Error! Could not convert " << argv[5] << " to an integer!" << endl;
				exit(EXIT_FAILURE);
			}
		case 5:
			try {
				bufferSize = stoi(argv[4]);
			} catch (std::invalid_argument e) {
				cerr << "Error! Could not convert " << argv[4] << " to an integer!" << endl;
				exit(EXIT_FAILURE);
			}
		case 4:
			try {
				produce = stoi(argv[3]);
			} catch (std::invalid_argument e) {
				cerr << "Error! Could not convert " << argv[3] << " to an integer!" << endl;
				exit(EXIT_FAILURE);
			}
		case 3:
			try {
				prods = stoi(argv[2]);
			} catch (std::invalid_argument e) {
				cerr << "Error! Could not convert " << argv[2] << " to an integer!" << endl;
				exit(EXIT_FAILURE);
			}
		case 2:
			try {
				cons = stoi(argv[1]);
			} catch (std::invalid_argument e) {
				cerr << "Error! Could not convert" << argv[1] << " to an integer!" << endl;
				exit(EXIT_FAILURE);
			}
		case 1:
			break;
		default:
			cerr << "Usage: boundedBuffer [ Cons [ Prods [ Produce [ BufferSize [ Delays ] ] ] ] ]" << endl;
			exit(EXIT_FAILURE);
	}

#ifdef __U_MULTI__
	uProcessor p[3] __attribute__((unused)); // create 3 kernel thread for a total of 4
#endif // __U_MULTI__
	
	BoundedBuffer<BTYPE> boundedBuffer(bufferSize);
	
	// start producers
	Producer *producers[prods];
	for (int p=0; p<prods; p+=1) {
		producers[p] = new Producer(boundedBuffer, produce, delays);
	}

	// start consumers
	Consumer *consumers[cons];
	int sums[cons];
	for (int c=0; c<cons; c+=1) {
		sums[c] = 0;
		consumers[c] = new Consumer(boundedBuffer, delays, SENTINEL, sums[c]);
	}

	// wait for producers to finish
	for (int p=0; p<prods; p+=1) {
		delete producers[p];
	}
	
	// insert sentinel values to terminate consumers
	for (int c=0; c<cons; c+=1) {
		boundedBuffer.insert(SENTINEL);
	}
	
	// delete consumers and compute total
	int total = 0;
	for (int c=0; c<cons; c+=1) {
		delete consumers[c];
		total += sums[c];
	}
	cout << "total: " << total << endl;
}

