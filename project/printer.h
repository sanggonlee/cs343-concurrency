#ifndef __PRINTER_H__
#define __PRINTER_H__

_Monitor Printer {
	unsigned int numStudents;
	unsigned int numVendingMachines;
	unsigned int numCouriers;
	unsigned int numEntities;
	char *buffer;
	
	void resetBuffer();
	char intToChar(unsigned int n);
	void printOut();
	void checkAndPrint(unsigned int location);
	void fillFinished();
  public:
    enum Kind { Parent, Groupoff, WATCardOffice, NameServer, Truck, BottlingPlant, Student, Vending, Courier };
    Printer( unsigned int numStudents, unsigned int numVendingMachines, unsigned int numCouriers );
    ~Printer();
    void print( Kind kind, char state );
    void print( Kind kind, char state, int value1 );
    void print( Kind kind, char state, int value1, int value2 );
    void print( Kind kind, unsigned int lid, char state );
    void print( Kind kind, unsigned int lid, char state, int value1 );
    void print( Kind kind, unsigned int lid, char state, int value1, int value2 );
  private:
    unsigned int getBufferLocation(Kind kind);
	unsigned int getBufferLocation(Kind kind, unsigned int id);
};

#endif
