#ifndef __CONSUMER_H__
#define __CONSUMER_H__

#include <iostream>
#include "MPRNG.h"

extern MPRNG mPRNG;

_Task Consumer {
	static int tidGenerater;
	int tid;
	
	BoundedBuffer<int> &buffer;
	const int delay;
	const int sentinel;
	int &sum;

    void main() {
    	//uOwnerLock consOwnerLock;
    	for ( ;; ) {
    		yield(mPRNG(delay-1));
    		
    		//consOwnerLock.acquire();
//    		std::cout << "[Consumer]["<<tid<<"] removing " << std::endl;
    		int i = buffer.remove();
//    		std::cout << "[Consumer]["<<tid<<"] REMOVED " << i << std::endl;
    		//consOwnerLock.release();
    		
    		if (i == sentinel) return;
    		
    		//consOwnerLock.acquire();
    		sum += i;
    		//consOwnerLock.release();
    	}
    }
  public:
    Consumer( BoundedBuffer<int> &buffer, const int Delay, const int Sentinel, int &sum ) 
    : buffer(buffer)
    , delay(Delay)
    , sentinel(Sentinel)
    , sum(sum) {
    	tid = tidGenerater;
    	tidGenerater += 1;
    }
};

int Consumer::tidGenerater = 0;

#endif
