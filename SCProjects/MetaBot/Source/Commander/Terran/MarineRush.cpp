#include "MarineRush.h"
#include "../../Managers/Constructor.h"
#include "../../Managers/AgentManager.h"
#include "../../Evaluators/EconomyEvaluator.h"

MarineRush::MarineRush() : gameStage(INITIAL) {
	
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
	//gameStage = INITIAL;
	Broodwar->printf("Stage is %d", gameStage);
}


MarineRush::~MarineRush() {
}

void MarineRush::computeActions() {
	computeActionsBase();

	AgentManager* agentManager = AgentManager::getInstance();
	int currentSupply = Broodwar->self()->supplyUsed() / 2;
	int elapsedSeconds = Broodwar->elapsedTime();
	EconomyStrength economyStrength = EconomyEvaluator::getInstance()->evaluateEconomy();

	noWorkers = 15 * agentManager->countNoUnits(UnitTypes::Terran_Command_Center) + 2 * agentManager->countNoFinishedUnits(UnitTypes::Terran_Refinery);
	if (noWorkers > 60) noWorkers = 60;

	//select a worker to scout: between 1 and 10 minutes of game, with 8 supply, if nobody else is scouting
	if (scout1->size() == 0 && elapsedSeconds > 60 && elapsedSeconds < 600 && currentSupply == 8) {
		setUpScout();
	}

	//past 5 minutes, build an academy if necessary
	if (elapsedSeconds > 5*60 && Constructor::getInstance()->needBuilding(UnitTypes::Terran_Academy) && !inBuildPlan(UnitTypes::Terran_Academy)) {
		buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Academy, currentSupply));
	}

	//past 6 minutes, build an Engineering Bay if necessary
	if (elapsedSeconds > 6*60 && Constructor::getInstance()->needBuilding(UnitTypes::Terran_Engineering_Bay) && !inBuildPlan(UnitTypes::Terran_Engineering_Bay)) {
		buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Engineering_Bay, currentSupply));
	}

	//past 10 minutes, do default action
	if (elapsedSeconds > 10 * 60) {
		defaultAction();
	}

	/*
	Broodwar->printf("Stage %d", gameStage);
	gameStage = INITIAL;
	//from 5 minutes onwards, build academy, engineering bay and tech up marines
	if ((Broodwar->elapsedTime() / 60) > 5 && gameStage == INITIAL) {
		Broodwar->printf("INITIAL");
		buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Command_Center, 20));
		buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Academy, 22));
		buildplan.push_back(BuildplanEntry(TechTypes::Stim_Packs, 25));
		buildplan.push_back(BuildplanEntry(UpgradeTypes::U_238_Shells, 30));
		buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Engineering_Bay, 30));
		buildplan.push_back(BuildplanEntry(UpgradeTypes::Terran_Infantry_Weapons, 33));
		buildplan.push_back(BuildplanEntry(UpgradeTypes::Terran_Infantry_Armor, 35));

		Broodwar->printf("#academies: %d; #bays: %d", agentManager->countNoUnits(UnitTypes::Terran_Academy), agentManager->countNoUnits(UnitTypes::Terran_Engineering_Bay));
		if (agentManager->countNoUnits(UnitTypes::Terran_Academy) > 0 && agentManager->countNoUnits(UnitTypes::Terran_Engineering_Bay) > 0) {
			gameStage = UPGRADED;
			Broodwar->printf("Upgraded to stage %d", gameStage);
		}

		
	}

	else if (gameStage = UPGRADED) {
		//Broodwar->printf("UPGRADED");
		//Broodwar->printf("Stage is %d", gameStage);
		buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Command_Center, 40));
		buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Barracks, 40));
		buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Barracks, 50));
		buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Barracks, 60));
		buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Barracks, 70));
		buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Barracks, 80));

		//if (agentManager->countNoUnits(UnitTypes::Terran_Barracks) > 6) {
		gameStage = BARRACKS_UP;
		//}
	}

	else if (Broodwar->elapsedTime() / 60 > 20 || gameStage == BARRACKS_UP) {
		Broodwar->printf("default action");
		defaultAction();
	}
	*/

}