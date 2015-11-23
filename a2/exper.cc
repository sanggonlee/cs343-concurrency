#include <iostream>
using namespace std;


_Coroutine abc {
	
	void main() {
		suspend();
		cout << "in main" << endl;
		suspend();
	}
public:
	abc *B;
	void setB(abc *b) {
		B = b;
		B->r();
		cout << "in setB" << endl;
	}
	void r() {
		resume();
	}
};

void uMain::main() {
	abc* a = new abc;
	abc* b = new abc;
	a->r();
	b->r();
	
	a->setB(b);
	cout << "in uMain" << endl;
}
/*
_Coroutine Fc {
	void main() {
		cout << "[main] enter" << endl;
		
		mem();
		cout << "[main] after mem" << endl;
		
		resume();
		cout << "[main] after resume" << endl;
		
		suspend();
		cout << "[main] after suspend" << endl;
	}
public:
	void mem() { 
		cout << "[mem] before resume" << endl;
		resume();
		cout << "[mem] after resume" << endl;
	}
};
void uMain::main() {
	Fc fc;
	cout << "[uMain] before fc.mem()" << endl;
	fc.mem();
	cout << "[uMain] after fc.mem()" << endl;
}*/
