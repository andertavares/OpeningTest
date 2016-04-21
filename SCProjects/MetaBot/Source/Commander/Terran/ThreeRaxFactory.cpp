#include "ThreeRaxFactory.h"


ThreeRaxFactory::ThreeRaxFactory() {
	/* 
	 * BEGIN: 3 Rax Factory build order 
	 * This is the most used build order from AIIDE, CIG and SSCAIT 2015
	 */
	buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Supply_Depot, 9));
	buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Barracks, 10));
	buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Barracks, 15));
	buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Supply_Depot, 16));
	buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Supply_Depot, 20));
	buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Supply_Depot, 24));
	buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Barracks, 24));
	buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Refinery, 25));
	buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Supply_Depot, 26));
	buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Factory, 28));

	/* END: 3 Rax Factory */
}


ThreeRaxFactory::~ThreeRaxFactory() {
}
