// MyLedger.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <iomanip>
#include <mysql/jdbc.h>
#include <mysql.h>

#include "Data.h"
#include "Menu.h"
#include "MenuCommand.h"
#include "Account.h"
#include "Transaction.h"

using namespace std;

int addAcc();
void previewAccs();
int viewAcc();
void getDateInfo(int &, int &, int &);
int accSel();
int addTrans();
int viewTrans();
int recTrans();

int main()
{
	int fsid = -1;
	bool acct = false;
	bool trans = false;
	bool update = false;

	// Check for existing data
	Data* data = new Data();
	fsid = data->readFSID();
	if (fsid == 1) {
		data->firstStartSetup();
		fsid = 0;
		data->writeFSID(fsid);
	}

	// Set precision for monetary values
	cout << setprecision(2) << showpoint << fixed;

	// Menu object calls
	Menu* mainMenu;
	Menu* accMenu;
	Menu* transMenu;

	// Set up MySQL to query the number of accounts in DB
	try {
		sql::Driver* driver;
		sql::Connection* con;
		sql::PreparedStatement* pstmt;
		sql::ResultSet* res;

		driver = get_driver_instance();
		con = driver->connect("tcp://127.0.0.1:3306", "root", "password");
		con->setSchema("my_ledger");

		pstmt = con->prepareStatement("SELECT id FROM accounts");
		res = pstmt->executeQuery();
		while (res->next()) {
			if (res->getInt("id") > 0) {
				acct = true;
			}
		}

		pstmt = con->prepareStatement("SELECT id FROM transactions");
		res = pstmt->executeQuery();
		while (res->next()) {
			if (res->getInt("id") > 0) {
				trans = true;
			}
		}

		delete con;
		delete pstmt;
		delete res;
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

	// Show overview of accounts
	if (acct == true) {
		previewAccs();
	}

	// Menu Initialization
	mainMenu = new Menu("1", "Main Menu", "Q");
	accMenu = new Menu("1", "Accounts", "Q");
	mainMenu->addMenuItem(accMenu);
	accMenu->addMenuItem(new MenuCommand("1", "Add Accounts", addAcc));
	if (acct == true) {
		accMenu->addMenuItem(new MenuCommand("2", "View Accounts", viewAcc));
		transMenu = new Menu("2", "Transactions", "Q");
		mainMenu->addMenuItem(transMenu);
		transMenu->addMenuItem(new MenuCommand("1", "Add Transactions", addTrans));
		if (trans == true) {
			transMenu->addMenuItem(new MenuCommand("2", "View Transactions", viewTrans));
			transMenu->addMenuItem(new MenuCommand("3", "Reconcile Transactions", recTrans));
		}
	}

	//Menu execute
	mainMenu->execute();

	return EXIT_SUCCESS;
}

int addAcc() {
	int status = 0;
	const int MAX = 2;
	int accCount = -1;
	int num = 1;
	bool complete = false;
	bool valid = false;
	string inst = "";
	string name = "";
	string type = "";
	string active = "";
	double balance = 0.0;
	double initBalance = 0.0;
	int transData = -1;
	int transCount = -1;
	int intAccSel = 0;
	int intSel = 0;
	char charSel = ' ';
	string accType[MAX] = { "Checking", "Savings" };
	string filename = "N/A";;

	cout << "----------------------------------------------------------------------" << endl;
	cout << "Add Accounts" << endl << endl;

	do {
		// Get Institution name
		cin.ignore();
		cout << "Enter Institution name: ";
		getline(cin, inst);
		cout << endl << endl;

		// Get Account Name
		cout << "Enter Account name: ";
		getline(cin, name);
		cout << endl << endl;

		// Get Account type
		valid = false;
		while (!valid) {
			cout << "Select Account type:" << endl;
			num = 1;
			intSel = 0;
			for (int c = 0; c < MAX; c++) {
				cout << num++ << " - " << accType[c] << endl;
			}
			cin >> intSel;
			if (intSel < 1 || intSel > MAX) {
				cout << intSel << " is not a valid selection! Please try again!" << endl;
				cin.clear();
				cin.ignore();
				continue;
			}
			else {
				intSel--;
				type = accType[intSel];
				valid = true;
			}
		}
		cout << endl << endl;

		// Is Account active
		valid = false;
		while (!valid) {
			cout << "Is Account active? [Y/N] ";
			cin >> charSel;
			if (charSel == 'y' || charSel == 'Y' || charSel == 'n' || charSel == 'N') {
				valid = true;
			}
			else {
				cout << charSel << " is not a valid selection! Please try again!" << endl;
				continue;
			}
		}
		if (charSel == 'y' || charSel == 'Y') {
			active = "Yes";
		}
		else if (charSel == 'n' || charSel == 'N') {
			active = "No";
		}
		cout << endl << endl;

		// Get Account balance
		cout << "Enter Account balance: ";
		cin >> balance;
		initBalance = balance;
		cout << endl << endl;

		// Verify input
		valid = false;
		while (!valid) {
			cout << inst << " - " << name << " - " << type << " - " << active << " - " << balance << endl;
			cout << "Is this correct? [Y/N] ";
			cin >> charSel;
			if (charSel == 'y' || charSel == 'Y' || charSel == 'n' || charSel == 'N') {
				valid = true;
			}
			else {
				cout << charSel << " is not a valid selection! Please try again!" << endl;
				continue;
			}
		}
		if (charSel == 'n' || charSel == 'N') {
			continue;
		}
		else if (charSel == 'y' || charSel == 'Y') {
			// Add input to DB
			Account* Acc = new Account(inst, name, type, active, balance);
			
			// Ask user if another account needs to be added
			valid = false;
			while (!valid) {
				cout << "Another?: [Y/N]";
				cin >> charSel;
				if (charSel == 'y' || charSel == 'Y' || charSel == 'n' || charSel == 'N') {
					valid = true;
				}
				else {
					cout << charSel << " is not a valid selection! Please try again!" << endl;
					continue;
				}
			}
			if (charSel == 'y' || charSel == 'Y') {
				continue;
			}
			else if (charSel == 'n' || charSel == 'N') {
				complete = true;
			}
		}
	} while (!complete);

	return status;
}

void previewAccs() {
	Account* Acct = new Account();

	Acct->viewAccHeader();
	Acct->viewAccList();
	Acct->viewAccTotals();
}

int viewAcc() {
	int status = 0;
	Account* Acct = new Account();

	cout << "--------------------------------------------------" << endl << endl;
	cout << "View Accounts" << endl << endl;

	Acct->viewAccHeader();
	Acct->viewAccList();
	Acct->viewAccTotals();

	return status;
}

void getDateInfo(int &day, int &month, int &year) {
	time_t rawtime = time(NULL);
	struct tm* timeinfo = new tm;
	localtime_s(timeinfo, &rawtime);
	day = timeinfo->tm_mday;
	month = timeinfo->tm_mon + 1;
	year = timeinfo->tm_year + 1900;
}

int addTrans() {
	int status = 0;
	int count = 0;
	int num = 0;
	int intSel = 0;
	int intAccSel = 0;
	int intTypeSel = 0;
	int intMethodSel = 0;
	int destAccSel = 0;
	bool complete = false;
	bool valid = false;
	char charSel = ' ';
	string transStatus = "P";
	int day = 0;
	int month = 0;
	int year = 0;
	string name = "";
	string method = "";
	string cat = "";
	double amount = 0.0;
	double balance = 0.0;
	string destAccName = "";
	string destAccMethod = "Transfer";
	string destAccCat = "Transfer";
	double destTransAmount = 0.0;
	double destTransBal = 0.0;

	Account* Acct = new Account();
	Transaction* Trans = new Transaction();

	cout << "--------------------------------------------------" << endl << endl;
	cout << "Add Transaction" << endl << endl;

	while (!complete) {

		// Select Transaction type
		intTypeSel = Trans->getTransType();
		cout << endl << endl;

		// Select account
		intAccSel = Acct->accSel();
		if (intTypeSel == 3) {
			destAccSel = Acct->destAccSel(intAccSel);
		}

		// Get date info
		getDateInfo(day, month, year);

		//Get transaction name
		if (intTypeSel == 1 || intTypeSel == 2) {
			name = Trans->getTransName(intAccSel);
		}
		if (intTypeSel == 3) {
			destAccName = Trans->getDestAccName(intAccSel, destAccSel, name);
		}
		cout << endl << endl;

		// Get Transaction Method of payment
		method = Trans->getTransMethod(intTypeSel);
		cout << endl << endl;

		// Get Transaction Category
		cat = Trans->getTransCat(intTypeSel);
		cout << endl << endl;

		// Get Transaction amount
		amount = Trans->getTransAmount(intTypeSel);
		if (intTypeSel == 3) {
			destTransAmount = amount;
			amount = -amount;
		}
		cout << endl << endl;

		// Calaculate Transaction Balance
		balance = Trans->getTransBal(intAccSel, amount);
		cout << "balance: " << balance << endl;
		if (intTypeSel == 3) {
			destTransBal = Trans->getDestTransBal(destAccSel, destTransAmount);
		}

		// Verify input
		valid = false;
		while (!valid) {
			if (intTypeSel == 1 || intTypeSel == 2) { // Input for Expense or Income
				cout << intAccSel << " - " << month << "/" << day << "/" << year << " - " << transStatus << " - " << name << " - " << method << " - " << cat << " - " << amount << " - " << balance << endl;
			}
			else if (intTypeSel == 3) { // Two inputs for transfer
				cout << month << "/" << day << "/" << year << " - " << transStatus << " - " << name << " - " << method << " - " << cat << " - " << amount << " - " << balance << endl;
				cout << month << "/" << day << "/" << year << " - " << transStatus << " - " << destAccName << " - " << destAccMethod << " - " << destAccCat << " - " << destTransAmount << " - " << destTransBal << endl;
			}
			cout << "Is this correct? [Y/N]";
			cin >> charSel;
			if (charSel == 'y' || charSel == 'Y' || charSel == 'n' || charSel == 'N') {
				valid = true;
			}
			else {
				cout << intSel << " is not a valid selection! Please select Y or N.";
				cin.clear();
				cin.ignore();
				cout << endl << endl;
			}
		}
		if (charSel == 'n' || charSel == 'N') {
			continue;
		}

		// Update Account balance
		Acct->setAccBal(intAccSel, balance);
		if (intTypeSel == 3) {
			Acct->setAccBal(destAccSel, destTransBal);
		}

		// Make a new class object call for the transfer
		Transaction* DestTrans;

		// Add Transaction to DB
		if (intTypeSel == 1 || intTypeSel == 2) {
			Trans = new Transaction(intAccSel, transStatus, day, month, year, name, method, cat, amount, balance);
		}
		else if (intTypeSel == 3) {
			Trans = new Transaction(intAccSel, transStatus, day, month, year, name, method, cat, amount, balance);
			DestTrans = new Transaction(destAccSel, transStatus, day, month, year, destAccName, destAccMethod, destAccCat, destTransAmount, destTransBal);
		}

		// Ask if user if they want to enter another transaction
		valid = false;
		charSel = ' ';
		while (!valid) {
			cout << "Another? [Y/N]: ";
			cin >> charSel;
			cout << endl << endl;
			if (charSel == 'n' || charSel == 'N' || charSel == 'y' || charSel == 'Y') {
				valid = true;
			}
			else {
				cout << charSel << " is not a valid selection! Please select Y or N";
				cin.clear();
				cin.ignore();
				cout << endl << endl;
			}
		}
		if (charSel == 'y' || charSel == 'Y') {
			continue;
		}
		else if (charSel == 'n' || charSel == 'N') {
			complete = true;
		}
	}
	return 0;
}

int viewTrans() {
	int status = 0;
	int intAccSel = 0;

	// Make object calls for Account and Transaction
	Account* Acct = new Account();
	Transaction* Trans = new Transaction();

	// Select account for view transactions
	intAccSel = Acct->accSel();

	// View Transaction header and trasnaction list
	Trans->viewTransHeader();
	Trans->viewTrans(intAccSel);

	return status;
}

int recTrans() {
	bool complete = false;
	bool valid = false;
	char charSel = ' ';
	int status = 0;
	int intAccSel = 0;
	int intTransSel = 0;
	int day = 0;
	int month = 0;
	int year = 0;
	string transStatus = "";
	Account* Acct = new Account();
	Transaction* Trans = new Transaction();

	cout << "--------------------------------------------------" << endl << endl;
	cout << "Reconcile Pending Transactions" << endl << endl;

	// Select account
	intAccSel = Acct->accSel();
	cout << endl << endl;

	while (!complete) {
		try {
			// Call the SQL Driver, Connecttion, Prepared Statement, Result Set
			sql::Driver* driver;
			sql::Connection* con;
			sql::PreparedStatement* pstmt;
			sql::ResultSet* res;

			// Make a connection to the DB
			driver = get_driver_instance();
			con = driver->connect("tcp://127.0.0.1:3306", "root", "password");
			con->setSchema("my_ledger");

			// Check for pending transactions
			pstmt = con->prepareStatement("SELECT status FROM transactions WHERE status = 'P' AND acc_id = ?");
			pstmt->setInt(1, intAccSel);
			res = pstmt->executeQuery();

			while (res->next()) {
				if (res->getString("status") == "P") {
					break; // If there are pending transactions, break out of the while loop
				}
				else {
					// If there are no pending transactions, notify the user and set complete = true, then break out of the while loop
					cout << "There are no pending transactions at this time!" << endl;
					complete = true;
					break;
				}
			}

			// Select pending transaction to be reconciled
			intTransSel = Trans->selTrans(intAccSel);

			// Remove pending status
			pstmt = con->prepareStatement("UPDATE transactions SET status = '' WHERE id = ? AND acc_id = ?");
			pstmt->setInt(1, intTransSel);
			pstmt->setInt(2, intAccSel);
			pstmt->executeQuery();

			// Set new date for transaction
			cout << "Enter new date [dd/mm/yyyy]: ";
			cin >> day;
			cin >> month;
			cin >> year;

			// Update transaction date and status
			pstmt = con->prepareStatement("UPDATE transactions SET status = '', day = ?, month = ?, year = ? WHERE id = ? AND acc_id = ?");
			pstmt->setInt(1, day);
			pstmt->setInt(2, month);
			pstmt->setInt(3, year);
			pstmt->setInt(4, intTransSel);
			pstmt->setInt(5, intAccSel);
			pstmt->executeUpdate();

			Trans->viewTrans(intAccSel);

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
		// Ask user if they want to reconcile another transaction
		valid = false;
		charSel = ' ';
		while (!valid) {
			cout << "Another? [Y/N]: ";
			cin >> charSel;
			cout << endl << endl;
			if (charSel == 'n' || charSel == 'N' || charSel == 'y' || charSel == 'Y') {
				valid = true;
			}
			else {
				cout << charSel << " is not a valid selection! Please select Y or N";
				cin.clear();
				cin.ignore();
				cout << endl << endl;
			}
		}
		if (charSel == 'y' || charSel == 'Y') {
			continue;
		}
		else if (charSel == 'n' || charSel == 'N') {
			complete = true;
		}
		complete = true;
	}
	return status;
}