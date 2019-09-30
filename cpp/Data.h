#ifndef DATA_h
#define DATA_h

using namespace std;

class Data {
private:
public:
	Data() {}
	~Data(){}

	int readFSID();
	int writeFSID(int firstStartID);

	// Create new DB and Tables
	void firstStartSetup();
};

#endif