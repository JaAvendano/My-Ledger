#ifndef ACCOUNT_h
#define ACCOUNT_h

#include <string>

using namespace std;

class Account {
private:
public:
	Account(string inst, string name, string type, string active, double balance);
	Account() {}
	~Account() {}

	void viewAccHeader();
	void viewAccList();
	void viewAccTotals();

	double getAccBal(int);
	void setAccBal(int, double);

	int accSel();
	int destAccSel(int);
};

#endif