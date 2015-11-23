#include <iostream>
using namespace std;

int main() {
    char bytes[] = {
	// test valid 1-byte unicode
	0x7d, '\n',		// valid, value = 0x7d
	
	// test valid 1-byte unicode with extra characters
	0x52, 0x71, '\n',	// valid,  value = 0x52, extra = 0x71

	// test valid 2-byte unicode 
	0xc6, 0xa0, '\n',	// valid, 2-byte unicode, value = 0x1a
	
	// test invalid, expects 2 bytes but second byte missing
	0xd9, '\n',					// invalid

	// test empty line
	'\n',						// empty line
	
	// test invalid 1-byte character, doesn't fall into range
	0x85, '\n',					// invalid

	// test invalid - second byte not in a form of 10xxxxxx
	0xd0, 0xc0, '\n',				// invalid

	// test valid 3-byte unicode
	0xea, 0x92, 0xa5, '\n',				// valid, value = 0xa4a5

	// test valid 3-byte unicode with 3 extra bytes
	0xec, 0x87, 0xa3, 0xfe, 0x55, 0xb3, '\n',		// valid, value = 0xc1e3, extra = 0xfe55b3
	
	// test invalid - first byte not in any encoding types
	0xf8, '\n',				// invalid
	
	// test valid 4-byte unicode
	0xf4, 0x82, 0xa2, 0xbb, '\n',		// valid, value = 0x1028bb 
    };
    for ( int i = 0; i < sizeof( bytes ); i += 1 ) {
	cout << bytes[i];
    }
}

// Local Variables: //
// compile-command: "g++ assignment.cc" //
// End: //
