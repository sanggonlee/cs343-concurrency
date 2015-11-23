#include <iostream>
using namespace std;

volatile long int dummy1    // volatile prevents dead-code removal
#ifdef PAD
    __attribute__(( aligned (64) )) // align counters on 64-byte boundaries
#endif // PAD
    , counter1
#ifdef PAD
    __attribute__(( aligned (64) )) // align counters on 64-byte boundaries
#endif // PAD
    , counter2
#ifdef PAD
    __attribute__(( aligned (64) )) // align counters on 64-byte boundaries
#endif // PAD
    , dummy2
#ifdef PAD
    __attribute__(( aligned (64) )) // align counters on 64-byte boundaries
#endif // PAD
    ;

unsigned long int times = 100000000;

_Task Worker1 {
    void main() {
        for ( unsigned int i = 0; i < times; i += 1 ) {
            counter1 += 1;
        }
        cout << counter1 << endl;
    }
};
_Task Worker2 {
    void main() {
        for ( unsigned int i = 0; i < times; i += 1 ) {
            counter2 += 1;
        }
        cout << counter2 << endl;
    }
};
void uMain::main() {
    switch ( argc ) {
      case 2:
        times = atol( argv[1] );
    }
    cout << (void *)&dummy1 << ' ' << (void *)&counter1 << ' '
         << (void *)&counter2 << ' ' << (void *)&dummy2 << ' ' << endl;
	cout << sizeof(long int) << endl;
    uProcessor p;        // add virtual processor
    Worker1 w1;            // create threads
    Worker2 w2;
}
