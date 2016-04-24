#ifndef __CONFIGURATION_H__
#define __CONFIGURATION_H__

#include <string>

using namespace std;

class Configuration {
	//the singleton instance of this class
	static Configuration* instance;

	Configuration();

public:
	//identifies the build order the bot will use
	string buildOrderID;

	//path to write the file with match details
	string matchDataFile;

	//path to xml file with metagame definition
	string metaGamefile;

	//initial match speed
	int speed;

	//enable GUI (false makes match go much faster)
	bool enableGUI;

	//Returns the singleton instance of this class
	static Configuration* getInstance();

	~Configuration();

	void parseConfig();

	//Directory to read input from
	static const string INPUT_DIR;

	//Directory to write output to
	static const string OUTPUT_DIR;

	//File to read configs from
	static const string CONFIG_FILE;

	//directory that stores openings (build orders)
	static const string OPENINGS_DIR;

	//xml field that contains the build order ID
	static const string FIELD_BUILD_ORDER_ID;

	//xml field that contains the match data file
	static const string FIELD_MATCH_DATA_FILE;

	//xml field that contains the match data file
	static const string FIELD_METAGAME_FILE;

	//xml field that contains the initial match speed
	static const string FIELD_SPEED;

	//xml field that contains 'enable/disable' GUI
	static const string FIELD_ENABLE_GUI;
};

#endif
