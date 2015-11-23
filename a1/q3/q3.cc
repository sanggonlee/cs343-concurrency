#include <iostream>
#include <fstream>
using namespace std;

_Coroutine Utf8 {
  public:
    _Event Match {
      public:
        unsigned int unicode;
        Match( unsigned int unicode ) : unicode( unicode ) {}
    };
    _Event Error {};
  private:
    union UTF8 {
        unsigned char ch;                // character passed by cocaller
        struct {                         // types for 1st UTF-8 byte
            unsigned char dt : 7;        // data
            unsigned char ck : 1;        // check
        } t1;
        struct {
            unsigned char dt : 5;        // data
            unsigned char ck : 3;        // check
        } t2;
        struct {
            unsigned char dt : 4;
			unsigned char ck : 4;
		} t3;
        struct {
            unsigned char dt : 3;
			unsigned char ck : 5;
        } t4;
        struct {                         // type for extra UTF-8 bytes
        	unsigned char dt : 6;
			unsigned char ck : 2;
		} dt;
    } utf8;
    // YOU MAY ADD PRIVATE MEMBERS
	int numBytes;
	unsigned int data;
	int counter;
	istream *file;
	unsigned int input;
	unsigned int extra;

	void main() {

		for ( ;; ) {

			suspend();
		
			try {
        		if (utf8.ch == '\n') {
					
					if (input) { 
						cout << "0x" << std::hex << input << " : ";
					}

					if (numBytes < 0) {
						_Throw Error();
					} else if (data == 0) {
						cout << " : Warning! Blank line." << endl;
						setToDefault();
						continue;
					} else if (counter > 0) {
						_Throw Error();
					} else if ((numBytes == 1 && (data < 0x0 || data > 0x7F)) ||
							   (numBytes == 2 && (data < 0x80 || data > 0x7FF)) ||
							   (numBytes == 3 && (data < 0x800 || data > 0xFFFF)) ||
							   (numBytes == 4 && (data < 0x10000 || data > 0x10FFFF))){
						_Throw Error();
					} else {
						_Throw Match(data);
					}
				}

				if (counter == 0 || numBytes < 0) {
					// dealing with extra byte
					extra *= 256;
					extra += (unsigned int)utf8.ch;
					continue;
				}

				if (counter > 0 && utf8.dt.ck == 2) {
					input *= 256;
					input += (unsigned int)utf8.ch;
					data *= 64;		// ignore first 2 bits, so 256/4 = 64
					data += (unsigned int)utf8.dt.dt;
				} else if (utf8.t1.ck == 0 && numBytes == 0) {
					input = (unsigned int)utf8.ch;
					data = (unsigned int)utf8.t1.dt;
					numBytes = 1;
					counter = 1;
				} else if (utf8.t2.ck == 6 && numBytes == 0) {
					input = (unsigned int)utf8.ch;
					data = (unsigned int)utf8.t2.dt;
					numBytes = 2;
					counter = 2;
				} else if (utf8.t3.ck == 14 && numBytes == 0) {
					input = (unsigned int)utf8.ch;
					data = (unsigned int)utf8.t3.dt;
					numBytes = 3;
					counter = 3;
				} else if (utf8.t4.ck == 30 && numBytes == 0) {
					input = (unsigned int)utf8.ch;
					data = (unsigned int)utf8.t4.dt;
					numBytes = 4;
					counter = 4;
				} else if (numBytes == 0) {
					input = (unsigned int)utf8.ch;
					numBytes = -1;	// use it for handling ck not falling in any case
				} else if (counter > 0) {
					numBytes = -1;
					input *= 256;
					input += (unsigned int)utf8.ch;
				} else {
					extra *= 256;
					extra += (unsigned int)utf8.ch;
				}
					
				counter -= 1;

			} catch (Match) {
				cout << "valid 0x" << std::hex << data;
				if ((unsigned int)extra != 0) {
					cout << ". Extra characters 0x" << std::hex << extra;
				}
				cout << endl;
				setToDefault();
			} catch (Error) {
				cout << "invalid";
				if ((unsigned int)extra != 0) {
					cout << ". Extra characters 0x" << std::hex << extra;
				}
				cout << endl;
				setToDefault();
			}
		}
	}
  public:
    // YOU MAY ADD CONSTRUCTOR/DESTRUCTOR IF NEEDED
	void setToDefault() {
		input = 0;
		numBytes = 0;
		data = 0;
		counter = -1;
		extra = 0;
	}
	Utf8(istream *in) {
		file = in;
		setToDefault();
		resume();
	}
    void next( unsigned char c ) {
		utf8.ch = c;                     // insert character into union for analysis
		resume();
    }
};

void uMain::main() {

	istream *file = &cin;

	switch (argc) {
		case 2:
			try {
				file = new ifstream(argv[1]);
			} catch (uFile::Failure) {
				cerr << "Error while opening file" << endl;
				exit(EXIT_FAILURE);
			}
		case 1:
			break;
		default:
			cerr << "Usage: utf8 [ filename ]" << endl;
			exit(EXIT_FAILURE);
	}
	
	Utf8 utf(file);
	char c;
	
	for ( ;; ) {
		file->read(&c, sizeof(c));

		if (file->eof()) {
			delete file;
			break;
		}
		if (file->fail()) {
			cerr << "Error reading a byte" << endl;
			continue;
		}

		utf.next(c);
	}
}
