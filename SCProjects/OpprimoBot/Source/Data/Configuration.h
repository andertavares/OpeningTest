#ifndef __CONFIGURATION_H__
#define __CONFIGURATION_H__

#include <string>

using namespace std;

class Configuration {
	//identifies the build order the bot will use
	string buildOrderID;

	//path to write the file with match details
	string matchDataFile;

public:
	Configuration();
	~Configuration();
};

#endif
