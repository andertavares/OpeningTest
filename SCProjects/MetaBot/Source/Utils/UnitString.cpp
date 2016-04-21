#include "UnitString.h"


UnitString::UnitString() {
}


UnitString::~UnitString() {
}

string UnitString::shortName(UnitType type) {
	string name = type.getName();
	string res = type.getName();

	string raceName = type.getRace().getName();
	if (name.find(raceName) == 0) {
		int i = name.find("_");
		res = name.substr(i + 1, name.length());
	}
	//res.replace(res.begin(), res.end(), '_', ' ');

	if (res == "Siege_Tank_Tank_Mode") res = "Tank";
	if (res == "Siege_Tank_Siege_Mode") res = "Tank (Siege)";
	return res;
}
