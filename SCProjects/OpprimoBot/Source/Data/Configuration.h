#ifndef __CONFIGURATION_H__
#define __CONFIGURATION_H__

#include <string>

using namespace std;

class Configuration {
	//identifies the build order the bot will use
	string buildOrderID;

	//path to write the file with match details
	string matchDataFile;

	//the singleton instance of this class
	static Configuration* instance;

	Configuration();

public:
	//Returns the singleton instance of this class
	Configuration* getInstance();

	~Configuration();

	void parseConfig();

	//Directory to read input from
	static const string INPUT_DIR;

	//Directory to write output to
	static const string OUTPUT_DIR;

	//File to read configs from
	static const string CONFIG_FILE;
};

#endif
