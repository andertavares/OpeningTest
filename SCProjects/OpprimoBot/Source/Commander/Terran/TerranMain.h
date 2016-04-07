#ifndef __TERRANMAIN_H__
#define __TERRANMAIN_H__

#include "../Commander.h"
#include "../Squad.h"

using namespace BWAPI;
using namespace std;

/**  This is the Commander class for a defensive Marine/Siege Tank/Goliath
 * based strategy.
 *
 * Author: Johan Hagelback (johan.hagelback@gmail.com)
 */
class TerranMain : public Commander {

private:
	Squad* mainSquad;
	Squad* secondarySquad;
	Squad* backupSquad1;
	Squad* backupSquad2;
	Squad* scout1;
	Squad* scout2;

	/** time (seconds) since last evaluation */
	int lastTimeChecked;
	
public:
	TerranMain();

	/** Destructor. */
	~TerranMain();

	/** Called each update to issue orders. */
	virtual void computeActions();

	/** Actions to do when game condition doesn't match the expected ones in computeActions */
	void defaultAction();

	/** Train more military units */
	void enhanceMilitary();

	/** Research some techs / advance in Tech Tree */
	void techUp();

	/** Research some upgrades so that units get better/stronger */
	void doUpgrades();

	/** Create new base */
	void expand();

	/** Returns the unique id for this strategy. */
	static string getStrategyId()
	{
		return "TerranMain";
	}
};

#endif
