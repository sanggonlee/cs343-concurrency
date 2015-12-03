#ifndef __PARENT_H__
#define __PARENT_H__

#include "MPRNG.h"
#include "printer.h"
#include "bank.h"

extern MPRNG mprng;

_Task Parent {
	Printer &printer;
	Bank &bank;
	unsigned int numStudents;
	unsigned int parentalDelay;

    void main();
  public:
    Parent( Printer &prt, Bank &bank, unsigned int numStudents, unsigned int parentalDelay );
    ~Parent();
};

#endif
