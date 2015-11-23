#ifndef __PRODUCER_H__
#define __PRODUCER_H__

#include <iostream>
#include "MPRNG.h"

extern MPRNG mPRNG;

_Task Producer {
	static int tidGenerater;
	int tid;
	
	BoundedBuffer<int> &buffer;
	const int produce;
	const int delay;

    void main() {
    	//uOwnerLock prodOwnerLock;
    	for (int i=1; i<=produce; i+=1) {
    		yield(mPRNG(delay-1));
    		
    		//prodOwnerLock.acquire();
//    		std::cout << "[Producer]["<<tid<<"] INSERT " << i << std::endl;
    		buffer.insert(i);
//    		std::cout << "[Producer]["<<tid<<"] DONE WITH " << i << std::endl;
    		//prodOwnerLock.release();
    	}
    }
  public:
    Producer( BoundedBuffer<int> &buffer, const int Produce, const int Delay ) 
    : buffer(buffer) 
    , produce(Produce) 
    , delay(Delay) {
        tid = tidGenerater;
    	tidGenerater += 1;
    }
};

int Producer::tidGenerater = 0;

#endif
