#ifndef __THREERAXFACTORY_H__
#define __THREERAXFACTORY_H__

#include "TerranMain.h"
class ThreeRaxFactory : public TerranMain {
public:
	ThreeRaxFactory();
	~ThreeRaxFactory();

	/** Returns the unique id for this strategy. */
	static string getStrategyId() {
		return "3 Rax Factory";
	}
};

#endif