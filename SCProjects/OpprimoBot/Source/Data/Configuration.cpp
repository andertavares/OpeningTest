#include "Configuration.h"
#include "../Utils/tinyxml2.h"
#include <BWAPI.h>

//change these if you need to read/write in different folders
const string Configuration::INPUT_DIR = "bwapi-data/AI/";
const string Configuration::OUTPUT_DIR = "bwapi-data/AI/";
const string Configuration::CONFIG_FILE = Configuration::INPUT_DIR + "config.xml";

Configuration* Configuration::instance = NULL;

Configuration::Configuration() {
	//sets up default values
	matchDataFile = OUTPUT_DIR + "output.xml";
	buildOrderID = "Quick Factory Mines";
}

Configuration* Configuration::getInstance() {
	if (instance == NULL) {
		instance = new Configuration();
	}
	return instance;
}


Configuration::~Configuration() {
}


void Configuration::parseConfig() {
	using namespace tinyxml2;
	using namespace BWAPI;

	XMLDocument doc;
	int result = doc.LoadFile(CONFIG_FILE.c_str());

	if (result != XML_NO_ERROR) {
		Broodwar->printf("An error has occurred while parsing the configuration file '%s'. Error: '%s'", CONFIG_FILE.c_str(), doc.ErrorName());
		return;
	}

	//if matchDataFile <<
	//if buildOrderID <<


}