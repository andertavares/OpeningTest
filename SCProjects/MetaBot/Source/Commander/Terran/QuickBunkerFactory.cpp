#include "QuickBunkerFactory.h"


QuickBunkerFactory::QuickBunkerFactory() {
	/*
	 * BEGIN: Quick Bunker Factory build order
	 * This is the 3rd most used build order from AIIDE, CIG and SSCAIT 2015
	 */
	buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Supply_Depot, 9));
	buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Barracks, 10));
	buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Refinery, 10));
	buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Bunker, 12));
	buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Supply_Depot, 16));
	buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Bunker, 16));
	buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Factory, 16));
	buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Academy, 20));
	buildplan.push_back(BuildplanEntry(TechTypes::Tank_Siege_Mode, 20));

	/* END: Quick Bunker Factory */
}


QuickBunkerFactory::~QuickBunkerFactory() {
}
