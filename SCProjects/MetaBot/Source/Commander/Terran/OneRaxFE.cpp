#include "OneRaxFE.h"


OneRaxFE::OneRaxFE() {
	/* BEGIN: 1 Rax FE build order */

	buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Supply_Depot, 9));
	buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Barracks, 11));
	buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Command_Center, 15));
	buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Supply_Depot, 16));
	//this below is an extension
	buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Bunker, 16));
	buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Bunker, 22));
	buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Supply_Depot, 24));
	buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Supply_Depot, 32));

	/* END: 1 Rax FE */
}


OneRaxFE::~OneRaxFE() {
	for (Squad* s : squads) {
		delete s;
	}
	instance = NULL;
}
