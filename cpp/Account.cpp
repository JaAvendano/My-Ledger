#include "Account.h"

#include <string>
#include <mysql/jdbc.h>
#include <iomanip>
#include <vector>

using namespace std;

using namespace std;

Account::Account(string inst, string name, string type, string active, double balance) {
	try {
		// Set pointers for MySQL classes
		sql::Driver* driver;
		sql::Connection* con;
		sql::PreparedStatement* pstmt;

		// Connection
		driver = get_driver_instance();
		con = driver->connect("tcp://127.0.0.1:3306", "root", "password");
		con->setSchema("my_ledger");

		// Prepared Statment
		pstmt = con->prepareStatement("INSERT INTO accounts(institution, type, name, active, balance) VALUES (?,?,?,?,?)");
		pstmt->setString(1, inst);
		pstmt->setString(2, type);
		pstmt->setString(3, name);
		pstmt->setString(4, active);
		pstmt->setDouble(5, balance);
		pstmt->execute();
		delete con;
		delete pstmt;
	}
	catch (sql::SQLException &e) {
		cout << "# ERR: SQLException in " << __FILE__;
		cout << "(" << __FUNCTION__ << ") on line "
			<< __LINE__ << endl;
		cout << "# ERR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() <<
			" )" << endl;
	}
}

void Account::viewAccHeader() {
	cout << "\n\n|------------------------------------------------------------------|\n";
	cout << "| # ";
	cout << "| Institution " << "|" << setw(2);
	cout << " Account Name " << "|" << setw(13);
	cout << " Account Type " << "|" << setw(7);
	cout << " Active " << "|" << setw(9);
	cout << " Balance |\n";
	cout << "|------------------------------------------------------------------|\n";
}

void Account::viewAccList() {
	try {
		sql::Driver* driver;
		sql::Connection* con;
		sql::PreparedStatement* pstmt;
		sql::ResultSet* res;

		driver = get_driver_instance();
		con = driver->connect("tcp://127.0.0.1:3306", "root", "password");
		con->setSchema("my_ledger");

		pstmt = con->prepareStatement("SELECT * FROM accounts ORDER BY id");
		res = pstmt->executeQuery();
		while (res->next()) {
			cout << "|" << setw(2) << res->getInt("id");
			cout << " | " << setw(12) << left << res->getString("institution");
			cout << "| " << setw(13) << res->getString("name");
			cout << "| " << setw(13) << res->getString("type");
			cout << "| " << setw(7) << res->getString("active");
			cout << "| " << setw(8) << right << res->getDouble("balance") << "|" << endl;
		}
		delete con;
		delete res;
		delete pstmt;
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

void Account::viewAccTotals() {
	double balance = 0.0;
	int count = 0;

	try {
		sql::Driver* driver;
		sql::Connection* con;
		sql::Statement* stmt;
		sql::ResultSet* res;

		driver = get_driver_instance();
		con = driver->connect("tcp://127.0.0.1:3306", "root", "password");
		con->setSchema("my_ledger");

		stmt = con->createStatement();
		res = stmt->executeQuery("SELECT balance FROM accounts");
		vector<double> bal;
		while (res->next()) {
			bal.push_back(res->getDouble("balance"));
			count++;
		}
		delete con;
		delete res;
		delete stmt;

		for (int c = 0; c < count; c++) {
			balance += bal[c];
		}

		cout << "|------------------------------------------------------------------|" << endl;
		cout << "|" << setw(53) << "Total" << setw(6) << right << "$" << balance << "|" << endl;
		cout << "|------------------------------------------------------------------|\n" << endl;
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

double Account::getAccBal(int intAccSel) {
	double accBal = 0.0;

	try {
		sql::Driver* driver;
		sql::Connection* con;
		sql::PreparedStatement* pstmt;
		sql::ResultSet* res;

		// Set Connection
		driver = get_driver_instance();
		con = driver->connect("tcp://127.0.0.1:3306", "root", "password");
		con->setSchema("my_ledger");

		// Get current account balance
		pstmt = con->prepareStatement("SELECT balance from accounts WHERE id = ?");
		pstmt->setInt(1, intAccSel);
		res = pstmt->executeQuery();

		while (res->next()) {
			accBal = res->getDouble("balance");
		}
		delete con;
		delete res;
		delete pstmt;
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
	return accBal;
}

void Account::setAccBal(int intAccSel, double balance) {
	try {
		sql::Driver* driver;
		sql::Connection* con;
		sql::PreparedStatement* pstmt;

		// Set Connection
		driver = get_driver_instance();
		con = driver->connect("tcp://127.0.0.1:3306", "root", "password");
		con->setSchema("my_ledger");

		// Update current account balance
		pstmt = con->prepareStatement("UPDATE accounts SET balance = ? WHERE id = ?");
		pstmt->setDouble(1, balance);
		pstmt->setInt(2, intAccSel);

		pstmt->executeUpdate();
		delete con;
		delete pstmt;
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

int Account::accSel() {
	int intAccSel = 0;
	int count = 0;
	bool valid = false;

	cout << "--------------------------------------------------" << endl << endl;
	cout << "Account Select" << endl << endl;

	while (!valid) {
		try {
			sql::Driver* driver;
			sql::Connection* con;
			sql::PreparedStatement* pstmt;
			sql::ResultSet* res;

			driver = get_driver_instance();
			con = driver->connect("tcp://127.0.0.1:3306", "root", "password");
			con->setSchema("my_ledger");

			pstmt = con->prepareStatement("SELECT * FROM accounts ORDER BY id ASC");
			res = pstmt->executeQuery();

			cout << "Select an account:" << endl;
			while (res->next()) {
				cout << res->getInt("id") << " - " << res->getString("name") << endl;
				count++;
			}
			cin >> intAccSel;

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

		if (intAccSel < 1 || intAccSel > count) {
			cout << intAccSel << " is not a valid input! Please select a number from 1 to " << count;
			cin.clear();
			cin.ignore();
			cout << endl << endl;
		}
		else {
			valid = true;
		}
	}
	return intAccSel;
}

int Account::destAccSel(int intAccSel) {
	int destAccSel = 0;
	int count = 0;
	bool valid = false;
	bool sel = false;

	cout << "--------------------------------------------------" << endl << endl;
	cout << "Account Select" << endl << endl;

	while (!valid) {
		try {
			sql::Driver* driver;
			sql::Connection* con;
			sql::PreparedStatement* pstmt;
			sql::ResultSet* res;

			driver = get_driver_instance();
			con = driver->connect("tcp://127.0.0.1:3306", "root", "password");
			con->setSchema("my_ledger");

			pstmt = con->prepareStatement("SELECT * FROM accounts ORDER BY id ASC");
			res = pstmt->executeQuery();

			cout << "Select *To* account:" << endl;
			while (res->next()) {
				cout << "IntAccSel: " << intAccSel << endl;
				if (res->getInt("id") == intAccSel) {
					cout << "X" << " - " << res->getString("name") << endl;
				}
				else {
					cout << res->getInt("id") << " - " << res->getString("name") << endl;
				}
				count++;
			}
			cin >> destAccSel;

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
		valid = true;
	}

	return destAccSel;
}