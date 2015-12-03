#ifndef __WATCARD_OFFICE_H__
#define __WATCARD_OFFICE_H__

#include <queue>
#include "MPRNG.h"
#include "printer.h"
#include "watcard.h"
#include "bank.h"

extern MPRNG mprng;

_Task WATCardOffice {
	Printer &printer;
	Bank &bank;
	unsigned int numCouriers;
	bool terminating;

    struct Job {                           // marshalled arguments and return future
        unsigned int studentId;
        unsigned int amount;
        WATCard *watCard;
        WATCard::FWATCard result;          // return future
        
        Job( unsigned int sId, unsigned int amount ) 
        	: studentId(sId), amount(amount), watCard(NULL) {}
        ~Job() {
        	//delete watCard;
        }
    };
    _Task Courier {
    	WATCardOffice *office;
    	Bank &bank;
    	Printer &printer;
    	unsigned int id;
    	
    	void main();	// communicates with bank
      public:
    	Courier(WATCardOffice* office, Bank &bank, Printer &prt, unsigned int id);
    	~Courier();
    	
    };

    void main();
  public:
    _Event Lost {};                        // lost WATCard
    WATCardOffice( Printer &prt, Bank &bank, unsigned int numCouriers );
    ~WATCardOffice();
    WATCard::FWATCard create( unsigned int sid, unsigned int amount );
    WATCard::FWATCard transfer( unsigned int sid, unsigned int amount, WATCard *card );
    Job *requestWork();
  private:
	Courier **couriers;
	std::queue<Job*> jobQueue;
};

#endif
