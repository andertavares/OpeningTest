#ifndef __MARINERUSH_H__
#define __MARINERUSH_H__

#include "TerranMain.h"

class MarineRush : public TerranMain {
public:
	MarineRush();
	~MarineRush();

	/** Returns the unique id for this strategy. */
	static string getStrategyId() {
		return "Marine Rush";
	}
};

#endif

