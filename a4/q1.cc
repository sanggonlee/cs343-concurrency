#include <cstdlib>                // atoi

int tasks = 1, times = 10000000;

_Task Worker {
    enum { size = 100 };
    void main() {
        for ( volatile int i = 0; i < times; i += 1 ) {
#ifdef HEAP
            volatile int *arr __attribute__ (( unused ));
            arr = new int[size];
            arr[0] = 0;
            delete [] arr;
#else // STACK
            volatile int arr[size] __attribute__ (( unused ));
            arr[0] = 0;
#endif
        }
    }
};

void uMain::main() {
    switch ( argc ) {
      case 3:
         times = atoi( argv[2] );
      case 2:
         tasks = atoi( argv[1] );
    }
    uProcessor p[tasks - 1];
    Worker workers[tasks];
}

// compile-command: "u++ new.cc -O2 -multi -nodebug" //
