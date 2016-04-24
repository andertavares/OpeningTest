#ifndef _PARSE_UTILS_H_
#define _PARSE_UTILS_H_

#include <string>
#include <iostream>
#include <fstream>

#include <BWAPI.h>
#include "rapidjson/document.h"
#include "JSONTools.h"
#include "Managers/BuildplanEntry.h"

namespace ParseUtils {
    const int UnitCode = 0;
    const int UpgradeCode = 1;
    const int TechCode = 2;

	/**
	 * Parses an opening/build-order file and returns the opening's 
	 * name contained in the file
	 */
    string ParseOpeningFile(const std::string & filename, std::vector<BuildplanEntry>& buildplan);
    int ParseType(const std::string& commandType, UnitType& unitype, TechType& techtype, UpgradeType& upgradetype);
}

#endif