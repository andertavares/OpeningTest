#include "QuickFactoryMines.h"

QuickFactoryMines::QuickFactoryMines() {
	/*
	* BEGIN: Quick Factory Mines build order
	* This is the 2nd most used build order from AIIDE, CIG and SSCAIT 2015
	*/
	buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Supply_Depot, 9));
	buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Barracks, 10));
	buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Refinery, 10));
	buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Factory, 14));
	buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Supply_Depot, 16));
	buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Bunker, 16));
	buildplan.push_back(BuildplanEntry(TechTypes::Spider_Mines, 18));
	buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Supply_Depot, 24));
	buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Factory, 24));

	/* END: Quick Factory Mines */
}


QuickFactoryMines::~QuickFactoryMines() {
}
