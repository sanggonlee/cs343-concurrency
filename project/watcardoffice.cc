#include "watcardoffice.h"

/****************************************
 *	WATCardOffice methods
 ****************************************/

/*
 *	Main routine for WATCardOffice
 *	WATCardOffice is an administrator task
 */
void WATCardOffice::main() {
	printer.print(Printer::Kind::WATCardOffice, 'S');
	for (unsigned int cId=0; cId<numCouriers; cId++) {
		couriers[cId] = new Courier(this, bank, printer, cId);
	}
	
	for ( ;; ) {
		_Accept(~WATCardOffice) {
			break;
		} or _When(!jobQueue.empty()) _Accept(requestWork) {
			printer.print(Printer::Kind::WATCardOffice, 'W');
		} or _Accept(create) {
		} or _Accept(transfer) {
		}
	}
	
	// Send Sentinel value through requestWork - Courier::main chain to terminate
	// tasks and successively delete Courier and WATCardOffice
	for (unsigned int c=0; c<numCouriers; c++) {
		jobQueue.push(NULL);
		_Accept(requestWork);
	}
}

/*
 *	Constructor for WATCardOffice
 */
WATCardOffice::WATCardOffice( Printer &prt, Bank &bank, unsigned int numCouriers )
: printer(prt)
, bank(bank)
, numCouriers(numCouriers) {
	couriers = new Courier*[numCouriers];
}

/*
 *	Destructor for WATCardOffice
 */
WATCardOffice::~WATCardOffice() {
	for (unsigned int cId=0; cId<numCouriers; cId++) {
		delete couriers[cId];
	}
	delete[] couriers;
	printer.print(Printer::Kind::WATCardOffice, 'F');
}

/*
 *	Generate a WATCard which is encapsulated by Job, put the Job into a queue,
 *	and pass the future WATCard to the caller Student
 *	A Courier later processes the Job and injects the real WATCard into the future
 */
WATCard::FWATCard WATCardOffice::create(unsigned int sid, unsigned int amount) {
	Job *job = new Job(sid, amount);
	job->watCard = new WATCard();
	jobQueue.push(job);
	printer.print(Printer::Kind::WATCardOffice, 'C', sid, amount);
	return job->result;
}

/*
 *	Encapsulate the given WATCard with Job, put the Job into a queue and pass
 *	the future WATCard to the caller Student
 *	A Courier later processes the Job and injects the given WATCard into the future
 */
WATCard::FWATCard WATCardOffice::transfer(unsigned int sid, unsigned int amount, WATCard *card) {
	Job *job = new Job(sid, amount);
	job->watCard = card;
	jobQueue.push(job);
	printer.print(Printer::Kind::WATCardOffice, 'T', sid, amount);
	return job->result;
}

/*
 *	Pops a Job in the front of the queue and pass it to a Courier
 */
WATCardOffice::Job *WATCardOffice::requestWork() {
	Job* job = jobQueue.front();
	jobQueue.pop();
	if (job == NULL) {
		// Got Sentinel value, pass it to Courier to indicate 
		// WATCardOffice is attempting to terminate
		return job;
	}
	return job;
}


/****************************************
 *	Courier methods
 ****************************************/

/*
 *	Main routine for Courier
 */
void WATCardOffice::Courier::main() {
	printer.print(Printer::Kind::Courier, id, 'S');
	for ( ;; ) {
		//_Accept(~Courier) {
		//	break;
		//} _Else {
			Job *job = office->requestWork();
			if (job == (Job*)NULL) {
				// Got Sentinel value, halt the task and wait for destructor
				delete job;
				break;
			}
		
			printer.print(Printer::Kind::Courier, id, 't', (int)job->studentId, (int)job->amount);
			bank.withdraw(job->studentId, job->amount);
			job->watCard->deposit(job->amount);
			printer.print(Printer::Kind::Courier, id, 'T', (int)job->studentId, (int)job->amount);
		
			if (mprng(5) == 0) {
				// Lost the WATCard. Insert exception to the future
				delete job->watCard;
				job->result.exception(new WATCardOffice::Lost);
			} else {
				job->result.delivery(job->watCard);
			}
			delete job;
		//}
	}
}

/*
 *	Constructor for Courier
 */
WATCardOffice::Courier::Courier(WATCardOffice* office, Bank &bank, Printer &prt, unsigned int id)
: office(office)
, bank(bank)
, printer(prt)
, id(id) {
}

/*
 *	Destructor for Courier
 *	Couriers get deleted when WATCardOffice is deleted
 */
WATCardOffice::Courier::~Courier() {
	printer.print(Printer::Kind::Courier, id, 'F');
}
