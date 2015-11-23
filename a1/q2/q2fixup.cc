#include <iostream>
#include <functional>
using namespace std;

#ifdef NOOUTPUT
#define print( x )
#else
#define print( x ) x
#endif


int C( int i, int j, std::function<void(int, int&, int&)> fixup ) {
    print( cout << i << " " << j << endl );
    fixup(1, i, j);
    print( cout << i << " " << j << endl );
    fixup(2, i, j);
    print( cout << i << " " << j << endl );
    fixup(3, i, j);
    return i;
}
int B( int i, int j, std::function<void(int, int&, int&)> fixup ) {
    if ( i > 0 ) B( i - 1, j, fixup );
    return C( i, j, fixup );
}
int A( int i, int j, int times ) {
    int iParameter = i;
	int k = 27, ret;
    for ( int i = 0; i < times; i += 1 ) {
        ret = B( i, j, [&] (int type, int &ei, int &ej) {
			switch (type) {
				case 1:
					ei = iParameter;
					ej = j;
					break;
				case 2:
					ei = k;
					break;
				case 3:
					print( cout << ei << " " << j << endl );
					break;
				default:
					break;
			}
		} );
    }
    return ret;
}
int main(int argc, char *argv[]) {
    long int m = 4, n = 6, times = 1;    // default values

    switch ( argc ) {
      case 4: times = atoi( argv[3] );
      case 3: n = atoi( argv[2] );
      case 2: m = atoi( argv[1] );
    } // switch
    cout << m << " " << n << " " << A( m, n, times ) << endl;
}
