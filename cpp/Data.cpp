#include "Data.h"

#include <iostream>
#include <fstream>
#include <exception>
#include <mysql/jdbc.h>

using namespace std;

int Data::readFSID() {
	int firstStartID = 1;
	ifstream inFile;
	bool complete = false;
	int status = 0;

	while (!complete) {
		try {
			inFile.open("data.dat", ios::in);
			if (!inFile) {
				cout << "There is no data to be read!" << endl;
				break;
			}
		}
		catch (ifstream::failure &e) {
			std::cout << "Exception caught (system_error):\n";
			std::cout << "Error: " << e.what() << '\n';
			std::cout << "Code: " << e.code().value() << '\n';
			std::cout << "Category: " << e.code().category().name() << '\n';
			std::cout << "Message: " << e.code().message() << '\n';
			status = e.code().value();
			break;
		}
		try {
			inFile >> firstStartID;
		}
		catch (ifstream::failure& e) {
			std::cout << "Exception caught (system_error):\n";
			std::cout << "Error: " << e.what() << '\n';
			std::cout << "Code: " << e.code().value() << '\n';
			std::cout << "Category: " << e.code().category().name() << '\n';
			std::cout << "Message: " << e.code().message() << '\n';
			break;
		}
		inFile.close();
		complete = true;
	}
	return firstStartID;
}

int Data::writeFSID(int firstStartID) {
	int status = 0;
	ofstream outFile;
	bool complete = false;

	while (!complete) {
		try {
			outFile.open("data.dat", ios::out);
			cout << "Opening data.dat for writing..." << endl;
		}
		catch (ofstream::failure& e) {
			std::cout << "Exception caught (system_error):\n";
			std::cout << "Error: " << e.what() << '\n';
			std::cout << "Code: " << e.code().value() << '\n';
			std::cout << "Category: " << e.code().category().name() << '\n';
			std::cout << "Message: " << e.code().message() << '\n';
			status = e.code().value();
			break;
		}
		try {
			outFile << firstStartID;
			cout << "Data succseefully written..." << endl;
		}
		catch (ofstream::failure& e) {
			std::cout << "Exception caught (system_error):\n";
			std::cout << "Error: " << e.what() << '\n';
			std::cout << "Code: " << e.code().value() << '\n';
			std::cout << "Category: " << e.code().category().name() << '\n';
			std::cout << "Message: " << e.code().message() << '\n';
			status = e.code().value();
			break;
		}
		complete = true;
	}
	cout << "Closing Connection!" << endl;
	outFile.close();
	return status;
}

void Data::firstStartSetup() {
	
	try {
		sql::Driver* driver;
		sql::Connection* con;
		sql::Statement* stmt;

		driver = get_driver_instance();
		con = driver->connect("tcp://127.0.0.1:3306", "root", "password");
		stmt = con->createStatement();
		stmt->execute("DROP SCHEMA IF EXISTS my_ledger");
		stmt->execute("CREATE SCHEMA my_ledger");
		cout << "Creating database..." << endl;
		con->setSchema("my_ledger");
		cout << "Creating db tables..." << endl;
		stmt->execute("CREATE TABLE accounts (id INT NOT NULL AUTO_INCREMENT, institution VARCHAR(100) NOT NULL, type VARCHAR(50) NOT NULL, name VARCHAR(50) NOT NULL, active VARCHAR(5) NOT NULL, balance DECIMAL(9,2) NOT NULL, PRIMARY KEY (id)) ENGINE = InnoDB DEFAULT CHARACTER SET = utf8;");
		stmt->execute("CREATE TABLE transactions (id INT NOT NULL AUTO_INCREMENT, acc_id INT NOT NULL, status VARCHAR(1), day INT NOT NULL, month INT NOT NULL, year INT NOT NULL, name VARCHAR(50) NOT NULL, method VARCHAR(50) NOT NULL, cat VARCHAR(50) NOT NULL, amount DECIMAL(9,2) NOT NULL, balance DECIMAL(9,2) NOT NULL, PRIMARY KEY (id, acc_id)) ENGINE = InnoDB DEFAULT CHARACTER SET = utf8;");
		stmt->execute("ALTER TABLE transactions ADD CONSTRAINT acc_types_fk FOREIGN KEY (acc_id) REFERENCES accounts(id) ON DELETE RESTRICT ON UPDATE CASCADE;");
		cout << "Done!" << endl;
		delete stmt;
		delete con;
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