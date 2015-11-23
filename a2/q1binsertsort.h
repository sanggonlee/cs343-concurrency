#ifndef __BINSERTSORT_H__
#define __BINSERTSORT_H__


template<typename T> _Coroutine Binsertsort {
    const T Sentinel;                // value denoting end of set
    T value;                         // communication: value being passed down/up the tree
    void main() {
    	T pivot = value;
	
	if (value == Sentinel) {
		suspend();
		return;
	}
	
	Binsertsort less(Sentinel);
	Binsertsort greater(Sentinel);

	// take unsorted values
	for ( ;; ) {
		suspend();	// prepare to take input
		
		if (value == Sentinel) {
			less.sort(Sentinel);
			greater.sort(Sentinel);
			break;
		}

		else if (value <= pivot) {
			less.sort(value);
		} else {
			greater.sort(value);
		}
	}
	suspend();
	Binsertsort::value = pivot;
	
	// get sorted values
	for ( ;; ) {
		Binsertsort::value = less.retrieve();
		if (value == Sentinel) {
			break;
		} else {
			suspend();
		}
	}
	
	Binsertsort::value = pivot;
	suspend();
	
	for ( ;; ) {
		Binsertsort::value = greater.retrieve();
		if (value == Sentinel) {
			break;
		} else {
			suspend();
		}
	}
    }
  public:
    Binsertsort( T Sentinel ) : Sentinel( Sentinel ) {}
    void sort( T value ) {           // value to be sorted
        Binsertsort::value = value;
        resume();
    }
    T retrieve() {                   // retrieve sorted value
        resume();
        return value;
    }
};

#endif
