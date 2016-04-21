#ifndef __ECONOMYEVALUATOR_H__
#define __ECONOMYEVALUATOR_H__

#include <BWAPI.h>
using namespace BWAPI;
using namespace std;

enum EconomyStrength {
	WEAK,		// low minerals or low gas
	FAIR,		// comfortable to produce some military
	STRONG,		// comfortable enough to expand
	ABUNDANT	// plenty of resources
};

enum ResourceLevels {
	LOW,			//under 100
	INTERMEDIATE,	//between 100 and COST_OF_BASE
	HIGH,			//more than COST_OF_BASE
	THOUSANDS		//more than 1000
};

class EconomyEvaluator {

private:
	static EconomyEvaluator* instance;
	
	EconomyEvaluator();


public:
	/** Destructor. */
	~EconomyEvaluator();

	/** Returns the instance to the Constructor that is currently used. */
	static EconomyEvaluator* getInstance();

	/** Returns an evaluation of how strong economy is currently */
	EconomyStrength evaluateEconomy();

	/** Classify current ammount of this resource */
	ResourceLevels evaluateMinerals();

	/** Classify current ammount of this resource */
	ResourceLevels evaluateGas();

	
};

#endif