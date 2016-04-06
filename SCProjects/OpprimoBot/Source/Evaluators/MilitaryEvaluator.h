#ifndef __MILITARYEVALUATOR_H__
#define __MILITARYEVALUATOR_H__

#include <BWAPI.h>
#include "../Managers/ExplorationManager.h"
using namespace BWAPI;
using namespace std;

enum MilitaryForce {
	NONE,			//no combat movements
	LIGHT_FEW,		//for land: infantry-like; for air: mutalisk-like (few units)
	LIGHT_MANY,		//same as above, but with many units
	HEAVY_FEW,		//for land: reaver-like; for air: battlecruiser-like (few units)
	HEAVY_MANY,		//same as above, but with many units
	MIXED_FEW,		//both light and heavy units are present
	MIXED_MANY		//same as above, but with many units
};

class MilitaryEvaluator {
private:
	static MilitaryEvaluator* instance;

	ExplorationManager* explorationManager;
	MilitaryEvaluator();

public:
	
	~MilitaryEvaluator();

	/** Returns the instance to the Constructor that is currently used. */
	static MilitaryEvaluator* getInstance();

	/** Returns whether prevalent combat units are light, heavy, mixed (or none) */
	MilitaryForce evaluateEnemyAir();

	/** Returns whether prevalent combat units are light, heavy, mixed (or none) */
	MilitaryForce evaluateEnemyLand();

	/** Returns whether opponent forces has light melee and should be dealt accordingly */
	bool shouldRespondLightMelee();
};

#endif