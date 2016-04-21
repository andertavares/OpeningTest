#ifndef __QUICKBUNKERFACTORY_H__
#define __QUICKBUNKERFACTORY_H__

#include "TerranMain.h"

class QuickBunkerFactory : public TerranMain {
public:
	QuickBunkerFactory();
	~QuickBunkerFactory();

	/** Returns the unique id for this strategy. */
	static string getStrategyId() {
		return "Quick Bunker Factory";
	}
};

#endif