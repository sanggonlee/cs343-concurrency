#ifndef __PRINTER_H__
#define __PRINTER_H__

class Printer {
    // YOU MAY ADD PRIVATE MEMBERS
	char *buffer;
	unsigned int numPlayers;
	unsigned int numCards;

	void resetBuffer();
	unsigned int encodeNumberToBuffer(unsigned int index, int num);
	void printOut(const unsigned int id);
  public:
    Printer( const unsigned int NoOfPlayers, const unsigned int NoOfCards );
    ~Printer();
    void prt( const unsigned int id, const unsigned int took, unsigned int PlayersRemaining );
};

#endif
