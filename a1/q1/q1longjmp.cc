#include <iostream>
#include <cstdlib>
using namespace std;
#include <unistd.h>                // getpid
#include <setjmp.h>

#ifdef NOOUTPUT
#define print( x )
#else
#define print( x ) x
#endif

struct E {};

long int freq = 5;

long int Ackermann( long int m, long int n, jmp_buf e ) {
    if ( m == 0 ) {
        if ( random() % freq == 0 ) longjmp(e, 1);//throw E();
        return n + 1;
    } else if ( n == 0 ) {
        if ( random() % freq == 0 ) longjmp(e, 2);//throw E();
        
        jmp_buf env1;
        if (!setjmp(env1)) {
            return Ackermann( m - 1, 1, env1 );
        } else {
            print( cout << "E1 " << m << " " << n << endl );
        }
    } else {
    	jmp_buf env2;
    	if (!setjmp(env2)) {
            return Ackermann( m - 1, Ackermann( m, n - 1, env2 ), env2 );
        } else {
            print( cout << "E2 " << m << " " << n << endl );
        }
    } // if
    return 0;    // recover by returning 0
}

int main( int argc, const char *argv[] ) {
    long int Ackermann( long int m, long int n, jmp_buf e );
    long int m = 4, n = 6, seed = getpid();    // default values

    switch ( argc ) {
      case 5: freq = atoi( argv[4] );
      case 4: seed = atoi( argv[3] );
      case 3: n = atoi( argv[2] );
      case 2: m = atoi( argv[1] );
    } // switch
    srandom( seed );
    cout << m << " " << n << " " << seed << " " << freq << endl;
    //try {
    
    jmp_buf env3;
    
    if (!setjmp(env3)) {
        cout << Ackermann( m, n, env3 ) << endl;
    } else {
        print( cout << "E3" << endl );
    }
}
