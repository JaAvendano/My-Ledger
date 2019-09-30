#ifndef TRANSACTION_h
#define TRANSACTION_h

#include <string>

using namespace std;

class Transaction {
private:
public:
	Transaction(int, string, int, int, int, string, string, string, double, double);
	Transaction(){}
	~Transaction(){}

	int getTransType();
	string getTransName(int);
	string getDestAccName(int, int, string&);
	string getTransMethod(int);
	string getTransCat(int);
	double getTransAmount(int);
	double getTransBal(int, double);
	double getDestTransBal(int, double);

	void viewTransHeader();
	void viewTrans(int);
	int selTrans(int);
};

#endif