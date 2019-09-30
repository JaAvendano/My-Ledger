#include "Transaction.h"

#include <iostream>
#include <string>
#include <mysql/jdbc.h>
#include <iomanip>
#include <exception>

using namespace std;

Transaction::Transaction(int intAccSel, string status, int day, int month, int year, string name, string method, string cat, double amount, double balance) {
	try {
		sql::Driver* driver;
		sql::Connection* con;
		sql::PreparedStatement* pstmt;

		// Set Connection
		driver = get_driver_instance();
		con = driver->connect("tcp://127.0.0.1:3306", "root", "password");
		con->setSchema("my_ledger");

		// Add input to DB
		pstmt = con->prepareStatement("INSERT INTO transactions(acc_id, status, day, month, year, name, method, cat, amount, balance) VALUES (?,?,?,?,?,?,?,?,?,?)");
		pstmt->setInt(1, intAccSel);
		pstmt->setString(2, status);
		pstmt->setInt(3, day);
		pstmt->setInt(4, month);
		pstmt->setInt(5, year);
		pstmt->setString(6, name);
		pstmt->setString(7, method);
		pstmt->setString(8, cat);
		pstmt->setDouble(9, amount);
		pstmt->setDouble(10, balance);

		pstmt->execute();

		delete pstmt;
		delete con;
	}
	catch (sql::SQLException& e) {
		cout << "# ERR: SQLException in " << __FILE__;
		cout << "(" << __FUNCTION__ << ") on line "
			<< __LINE__ << endl;
		cout << "# ERR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() <<
			" )" << endl;
	}
}

int Transaction::getTransType() {
	int intTypeSel = 0;
	int num = 1;
	int destAccSel = 0;
	const int MAX = 3;  // Types of transactions
	bool valid = false;
	string typeSel[MAX] = { "Expemse", "Deposit", "Transfer" };

	while (!valid) {
		cout << "Select Transaction type:" << endl;
		num = 1;
		for (unsigned int c = 0; c < MAX; c++) {
			cout << num++ << " - " << typeSel[c] << endl;
		}
		cin >> intTypeSel;
		cout << endl;
		if (intTypeSel < 1 || intTypeSel > MAX) {
			cout << intTypeSel << " is not a valid selection! Please select a number from 1 to " << MAX;
			cin.clear();
			cin.ignore();
			cout << endl << endl;
		}
		else {
			valid = true;
		}
	}
	return intTypeSel;
}

string Transaction::getTransName(int intAccSel) {
	string name = "";

	cin.ignore();
	cout << "Enter Transaction name: ";
	getline(cin, name);

	return name;
}

string Transaction::getDestAccName(int intAccSel, int destAccSel, string &name) {
	string destAccName = "";
	bool valid = false;
	int intSel = 0;
	int count = 0;
	string something = "";

	try {
		sql::Driver* driver;
		sql::Connection* con;
		sql::PreparedStatement* pstmt;
		sql::ResultSet* res;

		// Set Connection
		driver = get_driver_instance();
		con = driver->connect("tcp://127.0.0.1:3306", "root", "password");
		con->setSchema("my_ledger");

		pstmt = con->prepareStatement("SELECT name FROM accounts WHERE id = ?");
		pstmt->setInt(1, intAccSel);
		res = pstmt->executeQuery();

		while (res->next()) {
			name = res->getString("name");
		}

		pstmt = con->prepareStatement("SELECT name FROM accounts WHERE id = ?");
		pstmt->setInt(1, destAccSel);
		res = pstmt->executeQuery();
		while (res->next()) {
			destAccName = res->getString("name");
		}

		delete res;
		delete pstmt;
		delete con;
	}
	catch (sql::SQLException& e) {
		cout << "# ERR: SQLException in " << __FILE__;
		cout << "(" << __FUNCTION__ << ") on line "
			<< __LINE__ << endl;
		cout << "# ERR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() <<
			" )" << endl;
	}
	return destAccName;
}

string Transaction::getTransMethod(int intTypeSel) {
	bool valid = false;
	int num = 1;
	int intSel = 0;
	const int METHOD = 5; // Number of transaction methods
	string methodSel[METHOD] = { "Debit", "Credit", "Check", "Online", "Transfer" };
	string method = "";

	if (intTypeSel == 1) {
		valid = false;
		while (!valid) {
			cout << "Select method of payment:" << endl;
			num = 1;
			for (unsigned int c = 0; c < METHOD; c++) {
				cout << num++ << " - " << methodSel[c] << endl;
			}
			cin >> intSel;
			cout << endl;
			if (intSel < 1 || intSel > METHOD) {
				cout << intSel << " is not a valid input! Please select a number from 1 to " << METHOD;
				cin.clear();
				cin.ignore();
				cout << endl << endl;
			}
			else {
				valid = true;
			}
		}
		method = methodSel[--intSel];
		cout << endl << endl;
	}
	else if (intTypeSel == 2) {
		method = "N/A";
	}
	else if (intTypeSel == 3) {
		method = "Transfer";
	}
	return method;
}

string Transaction::getTransCat(int intTypeSel) {
	bool valid = false;
	int intSel = 0;
	int num = 1;
	const int EXP = 23; // number of expense catagories
	const int INC = 8; // number of deposit catagories
	string expense[EXP] = { "ATM", "Automotive", "Bills", "Clothing-Shoes", "Credit-Card", "Dining", "Education", "Electronics", "Entertainment", "Fee", "Gas", "Hobby", "Internet", "Loan", "Medical", "Misc", "Phone", "Rent", "Self-Care", "Services", "Subscriptions", "Taxes", "Transfer" };
	string income[INC] = { "Advance", "Cash", "Check", "Interest", "Paycheck", "Rebate", "Refund", "Transfer" };
	string cat = "";

	cout << "Select transaction category:" << endl;
	if (intTypeSel == 1) {
		valid = false;
		while (!valid) {
			num = 1;
			for (unsigned int c = 0; c < EXP; c++) {
				cout << num++ << " - " << expense[c] << endl;
			}
			cin >> intSel;
			cout << endl;
			if (intSel < 1 || intSel > EXP) {
				cout << intSel << " is not a valid input! Please select a number from 1 to " << EXP;
				cin.clear();
				cin.ignore();
				cout << endl << endl;
			}
			else {
				valid = true;
			}
		}
		cat = expense[--intSel];
	}
	else if (intTypeSel == 2) {
		valid = false;
		while (!valid) {
			num = 1;
			for (unsigned int c = 0; c < INC; c++) {
				cout << num++ << " - " << income[c] << endl;
			}
			cin >> intSel;
			cout << endl;
			if (intSel < 1 || intSel > INC) {
				cout << intSel << " is not a valid input! Please select a number from 1 to " << INC;
				cin.clear();
				cin.ignore();
				cout << endl << endl;
			}
			else {
				valid = true;
			}
		}
		cat = income[--intSel];
	}
	else if (intTypeSel == 3) {
		cat = "Transfer";
	}
	return cat;
}

double Transaction::getTransAmount(int intTypeSel) {
	double amount = 0.0;

	cout << "Enter transaction amount: ";
	cin >> amount;
	if (intTypeSel == 1) {
		amount = -amount;
	}
	return amount;
}

double Transaction::getTransBal(int intAccSel, double amount) {
	double balance = 0.0;

	try {
		sql::Driver* driver;
		sql::Connection* con;
		sql::PreparedStatement* pstmt;
		sql::ResultSet* res;

		driver = get_driver_instance();
		con = driver->connect("tcp://127.0.0.1:3306", "root", "password");
		con->setSchema("my_ledger");

		pstmt = con->prepareStatement("SELECT balance FROM transactions WHERE acc_id = 2 ORDER BY id DESC LIMIT 1");
		res = pstmt->executeQuery();
		while (res->next()) {
			balance = res->getDouble("balance");
		}

		if (balance == 0.0) {
			pstmt = con->prepareStatement("SELECT balance FROM accounts WHERE id  = ?");
			pstmt->setInt(1, intAccSel);
			res = pstmt->executeQuery();
			while (res->next()) {
				balance = res->getDouble("balance");
			}
		}
		balance = balance + amount;

		delete res;
		delete pstmt;
		delete con;
	}
	catch (sql::SQLException& e) {
		cout << "# ERR: SQLException in " << __FILE__;
		cout << "(" << __FUNCTION__ << ") on line "
			<< __LINE__ << endl;
		cout << "# ERR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() <<
			" )" << endl;
	}
	return balance;
}

double Transaction::getDestTransBal(int destAccSel, double destTransAmount) {
	double destTransBal = 0.0;

	try {
		sql::Driver* driver;
		sql::Connection* con;
		sql::PreparedStatement* pstmt;
		sql::ResultSet* res;
		
		driver = get_driver_instance();
		con = driver->connect("tcp://127.0.0.1:3306", "root", "password");
		con->setSchema("my_ledger");
		
		pstmt = con->prepareStatement("SELECT balance FROM transactions WHERE acc_id = ? ORDER BY id DESC LIMIT 1");
		pstmt->setInt(1, destAccSel);
		res = pstmt->executeQuery();
		while (res->next()) {
			destTransBal = res->getDouble("balance");
		}

		if (destTransBal == 0.0) {
			pstmt = con->prepareStatement("SELECT balance FROM accounts WHERE id = ?");
			pstmt->setInt(1, destAccSel);
			res = pstmt->executeQuery();
			while (res->next()) {
				destTransBal = res->getDouble("balance");
			}
		}

		destTransBal += destTransAmount;

		delete res;
		delete pstmt;
		delete con;
	}
	catch (sql::SQLException& e) {
		cout << "# ERR: SQLException in " << __FILE__;
		cout << "(" << __FUNCTION__ << ") on line "
			<< __LINE__ << endl;
		cout << "# ERR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() <<
			" )" << endl;
	}
	return destTransBal;
}

void Transaction::viewTransHeader() {
	cout << endl << endl;
	cout << "--------------------------------------------------" << endl << endl;
	cout << "View Transactions" << endl << endl;

	cout << "| # |";
	cout << setw(8) << left << " Status " << "|";
	cout << setw(12) << left << " Date " << "|";
	cout << setw(26) << left << " Name " << "|";
	cout << setw(11) << " Method " << "|";
	cout << setw(16) << " Category " << "|";
	cout << setw(10) << "  Amount " << "|";
	cout << setw(10) << " Balance " << "|" << endl;
}

void Transaction::viewTrans(int intAccSel) {
	try {
		sql::Driver* driver;
		sql::Connection* con;
		sql::PreparedStatement* pstmt;
		sql::ResultSet* res;

		driver = get_driver_instance();
		con = driver->connect("tcp://127.0.0.1:3306", "root", "password");
		con->setSchema("my_ledger");

		pstmt = con->prepareStatement("SELECT * FROM transactions WHERE acc_id = ? ORDER BY id DESC");
		pstmt->setInt(1, intAccSel);
		res = pstmt->executeQuery();

		while (res->next()) {
			cout << setw(4) << res->getString("status") << setw(5) <<  " |";
			cout << setw(2) << res->getInt("month") << "/" << res->getInt("day") << "/" << res->getInt("year") << setw(4) << " | ";
			cout << setw(24) << left << res->getString("name") << " | ";
			cout << setw(10) << res->getString("method") << "| ";
			cout << setw(15) << res->getString("cat") << "| ";
			cout << setw(9) << right << res->getDouble("amount") << "| ";
			cout << setw(9) << right << res->getDouble("balance") << "|";
			cout << endl;
		}

		delete res;
		delete pstmt;
		delete con;
	}
	catch (sql::SQLException& e) {
		cout << "# ERR: SQLException in " << __FILE__;
		cout << "(" << __FUNCTION__ << ") on line "
			<< __LINE__ << endl;
		cout << "# ERR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() <<
			" )" << endl;
	}
}

int Transaction::selTrans(int intAccSel) {
	int intTransSel = 0;

	try {
		sql::Driver* driver;
		sql::Connection* con;
		sql::PreparedStatement* pstmt;
		sql::ResultSet* res;

		driver = get_driver_instance();
		con = driver->connect("tcp://127.0.0.1:3306", "root", "password");
		con->setSchema("my_ledger");

		pstmt = con->prepareStatement("SELECT * FROM transactions WHERE acc_id = ? ORDER BY id DESC");
		pstmt->setInt(1, intAccSel);
		res = pstmt->executeQuery();

		while (res->next()) {
			cout << setw(3) << right << res->getInt("id") << " |";
			cout << setw(4) << res->getString("status") << setw(5) << " |";
			cout << setw(2) << res->getInt("month") << "/" << res->getInt("day") << "/" << res->getInt("year") << setw(4) << " | ";
			cout << setw(24) << left << res->getString("name") << " | ";
			cout << setw(10) << res->getString("method") << "| ";
			cout << setw(15) << res->getString("cat") << "| ";
			cout << setw(9) << right << res->getDouble("amount") << "| ";
			cout << setw(9) << right << res->getDouble("balance") << "|";
			cout << endl;
		}
		cin >> intTransSel;

		delete res;
		delete pstmt;
		delete con;
	}
	catch (sql::SQLException& e) {
		cout << "# ERR: SQLException in " << __FILE__;
		cout << "(" << __FUNCTION__ << ") on line "
			<< __LINE__ << endl;
		cout << "# ERR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() <<
			" )" << endl;
	}
	return intTransSel;
}