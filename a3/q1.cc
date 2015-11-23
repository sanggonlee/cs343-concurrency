#include <iostream>

using namespace std;

void printUsage() {
	cerr << "Usage: mergesort -s unsorted-file [sorted-file]" << endl
		 << "Usage: mergesort -t size (>=0) [ depth (>=0) ]" << endl;
}

void uMain::main() {
	istream *infile;
	ostream *outfile = &cout;
	unsigned int size;
	unsigned int depth = 0;

	switch (argc) {
		case 4:
			if (argv[1] == "-s") {
				try {
					outfile = new ofstream(argv[2]);
				} catch (uFile::Failure) {
					cerr << "Error! Could not open output file \"" << argv[2] << "\"" << endl;
					exit(EXIT_FAILURE);
				}
			} else if (argv[1] == "-t") {
				depth = atoi(argv[3]);
			} else {
				printUsage();
				exit(EXIT_FAILURE);
			}
		case 3:
			if (argv[1] == "-s") {
				try {
					infile = new ifstream(argv[1]);
				} catch (uFile::Failure) {
					cerr << "Error! Could not open input file \"" << argv[1] << "\"" << endl;
					exit(EXIT_FAILURE);
				}
			} else if (argv[1] == "-t") {
				size = atoi(argv[2]);
			} else {
				printUsage();
				exit(EXIT_FAILURE);
			}
			break;
		default:
			printUsage();
			exit(EXIT_FAILURE);
	}
	
	
	
	
	
	
	
	if (argv[1] == "-s") {
		delete infile;
		if (outfile != &cout) delete outfile;
	}
}
