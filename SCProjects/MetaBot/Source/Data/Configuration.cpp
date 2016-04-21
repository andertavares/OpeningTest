#include "Configuration.h"
#include "../Utils/tinyxml2.h"
#include <BWAPI.h>

//change these if you need to read/write in different folders
const string Configuration::INPUT_DIR = "bwapi-data/AI/";
const string Configuration::OUTPUT_DIR = "bwapi-data/AI/";
const string Configuration::CONFIG_FILE = Configuration::INPUT_DIR + "config.xml";

//xml field names
const string Configuration::FIELD_BUILD_ORDER_ID = "build-order";
const string Configuration::FIELD_MATCH_DATA_FILE = "match-output";
const string Configuration::FIELD_METAGAME_FILE = "metagame-file";
const string Configuration::FIELD_SPEED = "speed";
const string Configuration::FIELD_ENABLE_GUI = "gui";

Configuration* Configuration::instance = NULL;

Configuration::Configuration() {
	//sets up default values
	matchDataFile = OUTPUT_DIR + "output.xml";
	metaGamefile = INPUT_DIR + "metagame.xml";
	buildOrderID = "Quick Factory Mines";
	speed = 0;
	enableGUI = true;
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
		Broodwar->printf(
			"An error has occurred while parsing the configuration file '%s'. Error: '%s'", 
			CONFIG_FILE.c_str(), 
			doc.ErrorName()
		);
		return;
	}
	
	//sets build order
	XMLElement* buildOrder = doc.FirstChildElement("config")->FirstChildElement(FIELD_BUILD_ORDER_ID.c_str());
	if (buildOrder) {
		buildOrderID = string(buildOrder->Attribute("value"));
	}

	//sets speed
	XMLElement* speedElement = doc.FirstChildElement("config")->FirstChildElement(FIELD_SPEED.c_str());
	if (speedElement) {
		speedElement->QueryIntAttribute("value", &speed);
	}

	//sets gui
	XMLElement* guiElement = doc.FirstChildElement("config")->FirstChildElement(FIELD_ENABLE_GUI.c_str());
	if (guiElement) {
		guiElement->QueryBoolAttribute("value", &enableGUI);
	}

	//metagame file
	XMLElement* metaGameElement = doc.FirstChildElement("config")->FirstChildElement(FIELD_METAGAME_FILE.c_str());
	if (metaGameElement) {
		metaGamefile = string(metaGameElement->Attribute("value"));
	}




	//traverses the XML looking for configurations
	/*
	for (XMLNode* data = doc.FirstChild()->FirstChild(); data; data = data->NextSibling()) {
		if (data->ToText())
	}*/

	//if matchDataFile <<
	//if buildOrderID <<


}