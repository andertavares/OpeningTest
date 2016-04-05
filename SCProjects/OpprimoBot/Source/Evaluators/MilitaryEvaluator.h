#ifndef __MILITARYEVALUATOR_H__
#define __MILITARYEVALUATOR_H__

#include <BWAPI.h>
using namespace BWAPI;
using namespace std;

enum MilitaryWeight {
	NONE,	//no combat movements
	LIGHT,	//for land: infantry-like; for air: mutalisk-like
	HEAVY,	//for land: reaver-like; for air: battlecruiser-like
	MIXED,	//both light and heavy units are present
};

class MilitaryEvaluator {
private:
	static MilitaryEvaluator* instance;
	MilitaryEvaluator();

public:
	
	~MilitaryEvaluator();

	/** Returns the instance to the Constructor that is currently used. */
	static MilitaryEvaluator* getInstance();

	/** Returns whether prevalent combat units are light, heavy, mixed (or none) */
	MilitaryWeight evaluateEnemyAir();

	/** Returns whether prevalent combat units are light, heavy, mixed (or none) */
	MilitaryWeight evaluateEnemyLand();
};

#endif