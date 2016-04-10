#include "MarineRush.h"


MarineRush::MarineRush() {

	/**
	 * BEGIN: Marine Rush build order 
	 * This is the 4th most used build order from AIIDE, CIG and SSCAIT 2015
	 */
	buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Supply_Depot, 9));
	buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Barracks, 11));
	buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Barracks, 13));
	buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Supply_Depot, 16));
	buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Supply_Depot, 18));
	buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Supply_Depot, 20));
	buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Barracks, 22));
	buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Refinery, 22));
	buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Supply_Depot, 28));

	mainSquad->setSetup(UnitTypes::Terran_Marine, 120);
	mainSquad->setSetup(UnitTypes::Terran_Medic, 40);
	//mainSquad->setRequired(true);
	//mainSquad->setBuildup(true);
}


MarineRush::~MarineRush() {
}
