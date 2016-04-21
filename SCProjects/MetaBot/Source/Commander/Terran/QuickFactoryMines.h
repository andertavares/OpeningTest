#ifndef __QUICKFACTORYMINES_H__
#define __QUICKFACTORYMINES_H__

#include "TerranMain.h"

class QuickFactoryMines : public TerranMain {
public:
	QuickFactoryMines();
	~QuickFactoryMines();

	/** Returns the unique id for this strategy. */
	static string getStrategyId() {
		return "Quick Factory Mines";
	}
};

#endif
