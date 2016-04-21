#ifndef __UNITSTRING_H__
#define __UNITSTRING_H__

#include <string>
#include <BWAPI.h>

using namespace std;
using namespace BWAPI;

class UnitString {
	UnitString();
	~UnitString();
public:
	static string shortName(UnitType type);
};

#endif