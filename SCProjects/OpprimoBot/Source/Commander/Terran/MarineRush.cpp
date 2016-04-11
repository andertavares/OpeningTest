#include "MarineRush.h"
#include "../../Managers/AgentManager.h"

MarineRush::MarineRush() : stage(INITIAL) {
	
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

void MarineRush::computeActions() {
	computeActionsBase();

	AgentManager* agentManager = AgentManager::getInstance();
	int currentSupply = Broodwar->self()->supplyUsed() / 2;

	noWorkers = 15 * agentManager->countNoUnits(UnitTypes::Terran_Command_Center) + 3 * agentManager->countNoFinishedUnits(UnitTypes::Terran_Refinery);
	if (noWorkers > 60) noWorkers = 60;

	//from 5 minutes onwards, build academy, engineering bay and tech up marines
	if (Broodwar->elapsedTime() / 60 > 5 && stage == INITIAL) {
		
		buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Academy, 15));
		buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Engineering_Bay, 17));
		buildplan.push_back(BuildplanEntry(TechTypes::Stim_Packs, 18));
		buildplan.push_back(BuildplanEntry(UpgradeTypes::U_238_Shells, 20));
		buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Command_Center, 20));
		buildplan.push_back(BuildplanEntry(UpgradeTypes::Terran_Infantry_Weapons, 23));
		buildplan.push_back(BuildplanEntry(UpgradeTypes::Terran_Infantry_Armor, 30));

		if (agentManager->countNoUnits(UnitTypes::Terran_Academy) > 0 && agentManager->countNoUnits(UnitTypes::Terran_Engineering_Bay) > 0) {
			stage = UPGRADED;
		}

		
	}

	else if (stage = UPGRADED) {
		buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Command_Center, 40));
		buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Barracks, 40));
		buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Barracks, 50));
		buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Barracks, 60));
		buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Barracks, 70));
		buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Barracks, 80));

		//if (agentManager->countNoUnits(UnitTypes::Terran_Barracks) > 6) {
		stage = BARRACKS_UP;
		//}
	}

	else if (Broodwar->elapsedTime() / 60 > 20 || stage == BARRACKS_UP) {
		defaultAction();
	}


}