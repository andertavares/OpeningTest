#ifndef __MARINERUSH_H__
#define __MARINERUSH_H__

#include "TerranMain.h"

enum Stage {
	INITIAL,
	UPGRADED,
	BARRACKS_UP,
	LATE_GAME
};

class MarineRush : public TerranMain {
	Stage stage;	//counts steps to implement some actions

public:
	MarineRush();
	~MarineRush();

	virtual void computeActions();

	/** Returns the unique id for this strategy. */
	static string getStrategyId() {
		return "Marine Rush";
	}
};

#endif

