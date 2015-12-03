#ifndef __STUDENT_H__
#define __STUDENT_H__

#include "MPRNG.h"
#include "watcardoffice.h"
#include "nameserver.h"
#include "groupoff.h"

extern MPRNG mprng;

_Task Student {
	Printer &printer;
	NameServer &nameServer;
	WATCardOffice &cardOffice;
	Groupoff &groupoff;
	unsigned int id;
	unsigned int maxPurchases;

    void main();
  public:
    Student( Printer &prt, NameServer &nameServer, WATCardOffice &cardOffice, Groupoff &groupoff, unsigned int id, unsigned int maxPurchases );
};

#endif
