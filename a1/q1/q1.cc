#include <iostream>
#include <cstdlib>
using namespace std;
#include <unistd.h>                // getpid

#ifdef NOOUTPUT
#define print( x )
#else
#define print( x ) x
#endif

struct E {};

long int freq = 5;

long int Ackermann( long int m, long int n ) {
    if ( m == 0 ) {
        if ( random() % freq == 0 ) throw E();
        return n + 1;
    } else if ( n == 0 ) {
        if ( random() % freq == 0 ) throw E();
        try {
            return Ackermann( m - 1, 1 );
        } catch( E ) {
            print( cout << "E1 " << m << " " << n << endl );
        } // try
    } else {
        try {
            return Ackermann( m - 1, Ackermann( m, n - 1 ) );
        } catch( E ) {
            print( cout << "E2 " << m << " " << n << endl );
        } // try
    } // if
    return 0;    // recover by returning 0
}

int main( int argc, const char *argv[] ) {
    long int Ackermann( long int m, long int n );
    long int m = 4, n = 6, seed = getpid();    // default values

    switch ( argc ) {
      case 5: freq = atoi( argv[4] );
      case 4: seed = atoi( argv[3] );
      case 3: n = atoi( argv[2] );
      case 2: m = atoi( argv[1] );
    } // switch
    srandom( seed );
    cout << m << " " << n << " " << seed << " " << freq << endl;
    try {
        cout << Ackermann( m, n ) << endl;
    } catch( E ) {
        print( cout << "E3" << endl );
    } // try
}
