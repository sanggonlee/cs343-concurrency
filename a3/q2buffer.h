#ifndef __BOUNDED_BUFFER_H__
#define __BOUNDED_BUFFER_H__

#include <iostream>
using namespace std;

template<typename T> class BoundedBuffer {
	const unsigned int size;
	T *buffer;
	volatile unsigned int inIndex;
	volatile unsigned int outIndex;
	volatile unsigned int numElems;
	//uOwnerLock prodOwnerLock;
	//uOwnerLock consOwnerLock;
	uOwnerLock ownerLock;
	uCondLock prodCondLock;
	uCondLock consCondLock;
#ifdef NOBUSY
	uOwnerLock bargingOwnerLock;
	uCondLock bargingCondLock;
	volatile enum SignalState { 
		Signalling,
		NotSignalling
	} signalState;
	bool signalling;
#endif
  public:
    BoundedBuffer( const unsigned int size = 10 )
    : size(size) {
		buffer = new T[size];
		inIndex = 0;
		outIndex = 0;
		numElems = 0;
#ifdef NOBUSY
		signalState = SignalState::NotSignalling;
		signalling = false;
#endif
	}
	~BoundedBuffer() {
		delete[] buffer;
	}
#ifdef BUSY
	void insert( T elem ) {
		ownerLock.acquire();
		
		while (numElems == size) {
			prodCondLock.wait(ownerLock);
		}

		assert(numElems < size);
	
		buffer[inIndex] = elem;
		inIndex += 1;
		if (inIndex == size) inIndex = 0; 
		numElems += 1;
		
		consCondLock.signal();
	
		ownerLock.release();
	}
    T remove() {
    	ownerLock.acquire();
		
		while (numElems == 0) {
			consCondLock.wait(ownerLock);
		}
		
		assert(numElems > 0);
	
		T out = buffer[outIndex];
		outIndex += 1;
		if (outIndex == size) outIndex = 0;
		numElems -= 1;
		
		prodCondLock.signal();
	
		ownerLock.release();

		return out;
	}
#endif // BUSY

#ifdef NOBUSY
	void insert( T elem ) {
//cout << "[insert] enter, " << elem << endl;
		ownerLock.acquire();
		
		if (signalling/*signalState == SignalState::Signalling*/) {
//cout << "[insert] detect signalling, preventing barging, will wait" << endl;
			bargingCondLock.wait(ownerLock);
//			signalState = SignalState::NotSignalling;
			if (bargingCondLock.empty()) {
				signalling = false;
			}
		}
		
		if (numElems == size) {
//cout << "[insert] numElems == size" << endl;
			if (!bargingCondLock.empty()) {
				bargingCondLock.signal();
			}
			prodCondLock.wait(ownerLock);			// block
//cout << "[insert] set to NotSignalling" << endl;
//			signalState = SignalState::NotSignalling;	// returned and reacquired lock
			//signalling = false;
			//if (!bargingCondLock.empty()) {
//				signalState = SignalState::Signalling;
			//	signalling = true;
			//	bargingCondLock.signal();
			//}
		}

		assert(numElems < size);
	
		buffer[inIndex] = elem;
		
		inIndex += 1;
		if (inIndex == size) inIndex = 0; 
//cout << "############# numElems changes from " << numElems << " to " << numElems+1<<endl;
		numElems += 1;

		//if (!consCondLock.empty()) {
//cout << "[insert] signalling cons cond lock" << endl;
//			signalState = SignalState::Signalling;
			//signalling = true;
//cout << "		POPPING consCondLock" << endl;
			//consCondLock.signal();
		//}
		if (!consCondLock.empty()) {
			signalling = true;
			consCondLock.signal();
		} else if (!bargingCondLock.empty()) {
			signalling = true;
			bargingCondLock.signal();
		} else {
			signalling = false;
		}
		
		ownerLock.release();
	}





    T remove() {
//cout << "["<<tid<<"][remove] enter" << endl;
    
    	ownerLock.acquire();
//cout << "["<<tid<<"][remove] just acquired lock" << endl;
      
    	if (signalling/*signalState == SignalState::Signalling*/) {
//cout << "["<<tid<<"][remove] detect signalling, preventing barging, will wait" << endl;
    		bargingCondLock.wait(ownerLock);
//    		signalState = SignalState::NotSignalling;
			if (bargingCondLock.empty()) {
				signalling = false;
			}
//cout << "["<<tid<<"][remove] woke up from barging cond lock" << endl;
    	}
    	
      	if (numElems == 0) {
//cout << "["<<tid<<"][remove] numElems == 0" << endl;
//cout << "		["<<tid<<"][remove] pushing to consCondLock" << endl;
      		if (!bargingCondLock.empty()) {
				bargingCondLock.signal();
			}
			consCondLock.wait(ownerLock);			// block
//cout << "["<<tid<<"][remove] set to NotSignalling" << endl;
//      		signalState = SignalState::NotSignalling;	// returned and reacquired lock
			//signalling = false;
      		//if (!bargingCondLock.empty()) {
//      			signalState = SignalState::Signalling;
			//	signalling = true;
      		//	bargingCondLock.signal();
      		//}
      }

//cout << "numElems="<<numElems<<endl;
		assert(numElems > 0);
	
		T out = buffer[outIndex];
		
		outIndex += 1;
		if (outIndex == size) outIndex = 0;
//cout << "##########["<<tid<<"] numElems changes from " << numElems << " to " << numElems-1<<endl;
		numElems -= 1;
		
		if (!prodCondLock.empty()) {
//cout << "["<<tid<<"][remove] signalling prod cond lock" << endl;
//			signalState = SignalState::Signalling;
			signalling = true;
			prodCondLock.signal();
		} else if (!bargingCondLock.empty()) {
			signalling = true;
			bargingCondLock.signal();
		} else {
			signalling = false;
		}
//cout << "["<<tid<<"][remove] releasing lock, ret="<<out << endl;
		ownerLock.release();
	  
		return out;
	}
#endif // NOBUSY

};



#endif
