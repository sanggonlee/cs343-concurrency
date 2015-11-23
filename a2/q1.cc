#include "q1binsertsort.h"
#include <iostream>
#include <fstream>
using namespace std;


void uMain::main() {
	istream *infile;
	ostream *outfile = &cout;

	switch (argc) {
		case 3:
			try {
				outfile = new ofstream(argv[2]);
			} catch (uFile::Failure) {
				cerr << "Error! Could not open output file \"" << argv[2] << "\"" << endl;
				exit(EXIT_FAILURE);
			}
		case 2:
			try {
				infile = new ifstream(argv[1]);
			} catch (uFile::Failure) {
				cerr << "Error! Could not open input file \"" << argv[1] << "\"" << endl;
				exit(EXIT_FAILURE);
			}
			break;
		default:
			cerr << "Usage: binsertsort unsorted-file [ sorted-file ]" << endl;
			exit(EXIT_FAILURE);
	}


	
	for ( ;; ) {
		int num;
		Binsertsort<TYPE> bsort(SENTINEL);
		int numOfValues;
		*infile >> numOfValues;
		if (infile->fail()) break;
		
		// take unsorted
		for (int i=0; i<numOfValues; i+=1) {
			*infile >> num;
			cout << num << " ";
			bsort.sort(num);
		}
		cout << endl;
		bsort.sort(SENTINEL);
		
		// print sorted
		for (int j=0; j<numOfValues; j+=1) {
			TYPE sorted = bsort.retrieve();
			if (sorted == SENTINEL) {
				j--;
				continue;
			}
			*outfile << sorted << " ";
		}
		*outfile << endl << endl;
	}

	delete infile;
	if (outfile != &cout) delete outfile;
}
