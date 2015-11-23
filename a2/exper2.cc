#include <iostream>
using namespace std;

class Something {
	int a;
public:
	int get() {
		return a;
	}
	void set(int n) {
		a = n;
	}
};

class Another {
	Something some;
public:
	void ass(Something &s) {
		some = s;
	}
	int get() {
		return some.get();
	}
	void set(int n) {
		some.set(n);
	}
};


int main() {
	Another anth1;
	Another anth2;
	Something s1;
	s1.set(1);
	Something s2;
	s2.set(2);

	anth1.ass(s1);
	anth2.ass(s1);

	anth1.set(5);
	cout << anth1.get() << endl;
	cout << anth2.get() << endl;
}
