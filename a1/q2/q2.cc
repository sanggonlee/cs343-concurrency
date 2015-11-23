#include <iostream>
using namespace std;

#ifdef NOOUTPUT
#define print( x )
#else
#define print( x ) x
#endif

_Event E1 {
  public:
    int &i, &j;
    E1( int &i, int &j ) : i( i ), j( j ) {}
};
_Event E2 {
  public:
    int &i;
    E2( int &i ) : i( i ) {}
};
_Event E3 {
  public:
    int i;
    E3( int i ) : i( i ) {}
};

int C( int i, int j ) {
    print( cout << i << " " << j << endl );
    _Resume E1( i, j );
    print( cout << i << " " << j << endl );
    _Resume E2( j );
    print( cout << i << " " << j << endl );
    _Resume E3( 27 );
    return i;
}
int B( int i, int j ) {
    if ( i > 0 ) B( i - 1, j );
    return C( i, j );
}
int A( int i, int j, int times ) {
    int k = 27, ret;
    try {
        for ( int i = 0; i < times; i += 1 ) {
            ret = B( i, j );
        }
        return ret;
    } _CatchResume( E1 e ) {
        e.i = i;
        e.j = j;
    } _CatchResume( E2 e ) {
        e.i = k;
    } _CatchResume( E3 e ) {
        print( cout << e.i << " " << j << endl );
    }
}
void uMain::main() {
    long int m = 4, n = 6, times = 1;    // default values

    switch ( argc ) {
      case 4: times = atoi( argv[3] );
      case 3: n = atoi( argv[2] );
      case 2: m = atoi( argv[1] );
    } // switch
    cout << m << " " << n << " " << A( m, n, times ) << endl;
}
