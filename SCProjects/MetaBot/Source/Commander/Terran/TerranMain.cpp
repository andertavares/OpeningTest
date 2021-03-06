#include "TerranMain.h"
#include "../../Evaluators/EconomyEvaluator.h"
#include "../../Evaluators/MilitaryEvaluator.h"
#include "../../Managers/Constructor.h"
#include "../../Managers/BuildplanEntry.h"
#include "../../Managers/AgentManager.h"
#include "../../Managers/TechManager.h"
#include "../../Managers/ResourceManager.h"
#include "../../Managers/Upgrader.h"
#include "../../Managers/Constructor.h"
#include "../ExplorationSquad.h"
#include "../RushSquad.h"

TerranMain::TerranMain()
{
	/* BEGIN: standard Build Order */
	
	/*buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Supply_Depot, 8));
	buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Barracks, 9));
	buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Refinery, 12));
	buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Bunker, 14));
	buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Supply_Depot, 16));
	buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Bunker, 16));
	buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Factory, 18));
	buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Academy, 20));
	buildplan.push_back(BuildplanEntry(TechTypes::Tank_Siege_Mode, 22));
	buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Supply_Depot, 23));
	buildplan.push_back(BuildplanEntry(TechTypes::Stim_Packs, 29));
	buildplan.push_back(BuildplanEntry(UpgradeTypes::U_238_Shells, 30));
	buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Supply_Depot, 31));
	*/
	/* END: standard Build Order */



	mainSquad = new Squad(1, Squad::OFFENSIVE, "MainSquad", 10);
	mainSquad->addSetup(UnitTypes::Terran_Marine, 10);
	mainSquad->setRequired(true);
	mainSquad->setBuildup(true);
	squads.push_back(mainSquad);

	secondarySquad = new Squad(2, Squad::OFFENSIVE, "SecondarySquad", 10);
	secondarySquad->setRequired(false);
	secondarySquad->setBuildup(true);
	squads.push_back(secondarySquad);

	rush1 = new RushSquad(10, "RushSquad1", 11);
	rush1->setRequired(false);
	
	scout1 = new ExplorationSquad(11, "ScoutingSquad1", 11);
	scout1->setRequired(false);
	squads.push_back(scout1);

	backupSquad1 = new Squad(5, Squad::OFFENSIVE, "BackupSquad1", 11);
	backupSquad1->setRequired(false);
	backupSquad1->setBuildup(true);
	squads.push_back(backupSquad1);

	backupSquad2 = new Squad(6, Squad::OFFENSIVE, "BackupSquad2", 12);
	backupSquad2->setRequired(false);
	backupSquad2->setBuildup(true);
	squads.push_back(backupSquad2);

	noWorkers = 16;
	noWorkersPerRefinery = 3;
}

TerranMain::~TerranMain()
{
	for (Squad* s : squads)
	{
		delete s;
	}
	instance = NULL;
}

void TerranMain::computeActions()
{
	computeActionsBase();

	noWorkers = 15 * AgentManager::getInstance()->countNoUnits(UnitTypes::Terran_Command_Center) + 3 * AgentManager::getInstance()->countNoFinishedUnits(UnitTypes::Terran_Refinery);
	if (noWorkers > 60) noWorkers = 60;

	int cSupply = Broodwar->self()->supplyUsed() / 2;
	int minerals = Broodwar->self()->minerals();
	int gas = Broodwar->self()->gas();

	//select a worker to scout: between 1 and 10 minutes of game, with 8 supply, if nobody else is scouting
	if (scout1->size() == 0 && Broodwar->elapsedTime() > 60 && Broodwar->elapsedTime() < 600 && cSupply == 8) {
		setUpScout();
	}


	if (cSupply >= 20 && stage == 0)
	{
		mainSquad->addSetup(UnitTypes::Terran_Siege_Tank_Tank_Mode, 4);
		mainSquad->addSetup(UnitTypes::Terran_SCV, 1);
		mainSquad->addSetup(UnitTypes::Terran_Marine, 6);
		mainSquad->addSetup(UnitTypes::Terran_Medic, 4);

		stage++;
	}
	if (cSupply >= 30 && minerals > 200 && stage == 1)
	{
		buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Factory, cSupply));
		buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Armory, cSupply));
		buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Engineering_Bay, cSupply));

		mainSquad->addSetup(UnitTypes::Terran_Goliath, 3);
		mainSquad->setBuildup(false);

		stage++;
	}
	if (cSupply >= 45 && minerals > 200 && stage == 2)
	{
		buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Barracks, cSupply));
		buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Missile_Turret, cSupply));

		rush1->addSetup(UnitTypes::Terran_Vulture, 1);
		rush1->setPriority(1);
		rush1->setActivePriority(1000);
		squads.push_back(rush1);

		secondarySquad->addSetup(UnitTypes::Terran_Siege_Tank_Tank_Mode, 2);
		secondarySquad->addSetup(UnitTypes::Terran_Marine, 8);
		secondarySquad->addSetup(UnitTypes::Terran_Goliath, 2);
		secondarySquad->setBuildup(false);

		stage++;
	}
	if (Commander::getInstance()->getSquad(1)->isActive() && stage == 3)
	{
		buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Command_Center, cSupply));
		//ResourceManager::getInstance()->lockResources(UnitTypes::Terran_Command_Center);
		scout1->addSetup(UnitTypes::Terran_Vulture, 2);
		scout1->setBuildup(false);

		stage++;
	}
	
	if (stage == 4 && AgentManager::getInstance()->countNoUnits(UnitTypes::Terran_Command_Center) >= 2)
	{
		buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Missile_Turret, cSupply));
		buildplan.push_back(BuildplanEntry(UpgradeTypes::Terran_Vehicle_Weapons, cSupply));
		buildplan.push_back(BuildplanEntry(UpgradeTypes::Terran_Infantry_Weapons, cSupply));
		buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Starport, cSupply));
		buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Science_Facility, cSupply));

		stage++;
	}
	if (stage == 5 && AgentManager::getInstance()->countNoFinishedUnits(UnitTypes::Terran_Science_Facility) > 0)
	{
		buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Missile_Turret, cSupply));
		
		mainSquad->addSetup(UnitTypes::Terran_Science_Vessel, 1);

		buildplan.push_back(BuildplanEntry(TechTypes::Cloaking_Field, cSupply));

		backupSquad1->addSetup(UnitTypes::Terran_Wraith, 5);
		backupSquad1->setBuildup(false);

		stage++;
	}
	if (stage == 6 && AgentManager::getInstance()->countNoFinishedUnits(UnitTypes::Terran_Science_Vessel) > 0)
	{
		buildplan.push_back(BuildplanEntry(TechTypes::EMP_Shockwave, cSupply));
		buildplan.push_back(BuildplanEntry(UpgradeTypes::Terran_Vehicle_Weapons, cSupply));

		stage++;
	}
	if (stage == 7 && AgentManager::getInstance()->countNoFinishedUnits(UnitTypes::Terran_Physics_Lab) > 0)
	{
		backupSquad2->addSetup(UnitTypes::Terran_Battlecruiser, 2);
		backupSquad2->setBuildup(false);
		buildplan.push_back(BuildplanEntry(TechTypes::Yamato_Gun, cSupply));
		
		stage++;
	}
	else {
		//situation does not fit any previous conditions, will take some default actions
		defaultAction();
	}

}

void TerranMain::defaultAction(){
	if (/*buildplan.size() > 0 && */Broodwar->elapsedTime() < 60 * 10) return;	//skips doing anything if buildplan is not satisfied before 10 minutes
	if (Broodwar->elapsedTime() - lastTimeChecked < 5) return;	//check every 5 seconds
	
	lastTimeChecked = Broodwar->elapsedTime();

	Broodwar->printf("Default action...");
	EconomyStrength economyStrength = EconomyEvaluator::getInstance()->evaluateEconomy();
	int currentSupply = Broodwar->self()->supplyUsed() / 2;

	int numberSCVs = AgentManager::getInstance()->countNoUnits(UnitTypes::Terran_SCV);
	int numberCCs = AgentManager::getInstance()->countNoBases();
	/*Unitset myUnits = Broodwar->self()->getUnits();
	for (auto a : myUnits){
		if (a->getType() == UnitTypes::Terran_Command_Center)  numberCCs++;
	}*/
	/*if (numberSCVs < 60 && numberSCVs / numberCCs < 15){	//we want at least 15 SCVs per base at the limit of 60
		mainSquad->addSetup(UnitTypes::Terran_SCV, 1);

	}*/

	if (economyStrength == ABUNDANT){
		Broodwar->printf("Abundant economy :-D");
		techUp();
		doUpgrades();
		enhanceMilitary(true);
	}
	else if (economyStrength == STRONG){
		Broodwar->printf("Strong economy :-)");
		expand();
		techUp();
		enhanceMilitary(true);
	}
	else if (economyStrength == INTERMEDIATE){
		Broodwar->printf("Intermediate economy :-|");
		improveEconomy();
		enhanceMilitary(false);
	}
	else if (economyStrength == WEAK){
		Broodwar->printf("Weak economy :(");
		improveEconomy();
		//enhanceMilitary();
	}

	
}

void TerranMain::enhanceMilitary(bool force){
	AgentManager* agentManager = AgentManager::getInstance();
	int numberTanks = agentManager->countNoUnits(UnitTypes::Terran_Siege_Tank_Siege_Mode) + agentManager->countNoUnits(UnitTypes::Terran_Siege_Tank_Tank_Mode);
	int unloadedMarines = agentManager->countUnloadedUnits(UnitTypes::Terran_Marine);

	int numberMedics = agentManager->countNoUnits(UnitTypes::Terran_Medic);
	int numberGoliaths = agentManager->countNoUnits(UnitTypes::Terran_Goliath);
	int numberVultures = agentManager->countNoUnits(UnitTypes::Terran_Vulture);
	int numBunkers = agentManager->countNoUnits(UnitTypes::Terran_Bunker)
		+ Constructor::getInstance()->noInProduction(UnitTypes::Terran_Bunker);

	int maxBarracks = 8;
	int maxFactories = 4;
	int maxStarports = 4;

	int currentSupply = Broodwar->self()->supplyUsed() / 2;

	MilitaryForce enemyLand = MilitaryEvaluator::getInstance()->evaluateEnemyLand();
	MilitaryForce enemyAir = MilitaryEvaluator::getInstance()->evaluateEnemyAir();
	MilitaryForce enemyMech = MilitaryEvaluator::getInstance()->evaluateEnemyMech();

	EconomyStrength economy = EconomyEvaluator::getInstance()->evaluateEconomy();

	if (enemyLand == HEAVY_MANY || enemyLand == HEAVY_FEW || enemyLand == MIXED_MANY) {	//respond with tanks
		//techUpTo(Tank)
		TechManager::getInstance()->techUpTo(UnitTypes::Terran_Siege_Tank_Siege_Mode);
		Upgrader::getInstance()->addTech(TechTypes::Tank_Siege_Mode);
		mainSquad->addSetup(UnitTypes::Terran_Siege_Tank_Tank_Mode, 8);
		mainSquad->addSetup(UnitTypes::Terran_Marine, 10);
		mainSquad->addSetup(UnitTypes::Terran_Medic, 3);

		
		if (economy == STRONG || economy == ABUNDANT){
			if (AgentManager::getInstance()->countNoFinishedUnits(UnitTypes::Terran_Barracks) < maxBarracks) {
				buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Barracks, currentSupply));
			}
			if (AgentManager::getInstance()->countNoFinishedUnits(UnitTypes::Terran_Factory) < maxFactories) {
				buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Factory, currentSupply));
			}
		}
		

		Broodwar->printf("Responding to HEAVY enemy LAND.");
	}

	if (enemyLand == LIGHT_MANY || enemyLand == LIGHT_FEW ||  enemyLand == MIXED_FEW) {	//respond with infantry
		TechManager::getInstance()->techUpTo(UnitTypes::Terran_Firebat);
		TechManager::getInstance()->techUpTo(UnitTypes::Terran_Medic);
		mainSquad->addSetup(UnitTypes::Terran_Vulture, 6);
		//secondarySquad->addSetup(UnitTypes::Terran_Marine, 10);
		mainSquad->addSetup(UnitTypes::Terran_Medic, 1);
		mainSquad->addSetup(UnitTypes::Terran_Firebat, 3);	//TODO: check if Firebat is being micro'ed
		mainSquad->addSetup(UnitTypes::Terran_Marine, 6);
		
		if (economy == STRONG || economy == ABUNDANT){
			if (AgentManager::getInstance()->countNoFinishedUnits(UnitTypes::Terran_Barracks) < maxBarracks) {
				buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Barracks, currentSupply));
			}
			if (AgentManager::getInstance()->countNoFinishedUnits(UnitTypes::Terran_Factory) < maxFactories) {
				buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Factory, currentSupply));
			}
		}

		Broodwar->printf("Responding to LIGHT enemy LAND.");
	}

	if (enemyAir == HEAVY_MANY || enemyAir == HEAVY_FEW || enemyAir == MIXED_MANY) {	//respond with anti-air
		TechManager::getInstance()->techUpTo(UnitTypes::Terran_Goliath);
		TechManager::getInstance()->techUpTo(UnitTypes::Terran_Wraith);
		mainSquad->addSetup(UnitTypes::Terran_Goliath, 8);
		mainSquad->addSetup(UnitTypes::Terran_Wraith, 8);
		buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Missile_Turret, currentSupply));
		buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Missile_Turret, currentSupply));

		if (economy == STRONG || economy == ABUNDANT){
			if (AgentManager::getInstance()->countNoFinishedUnits(UnitTypes::Terran_Starport) < maxStarports) {
				buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Starport, currentSupply));
			}
			if (AgentManager::getInstance()->countNoFinishedUnits(UnitTypes::Terran_Factory) < maxFactories) {
				buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Factory, currentSupply));
			}
			Upgrader::getInstance()->addUpgrade(UpgradeTypes::Charon_Boosters);
			Upgrader::getInstance()->addUpgrade(UpgradeTypes::Terran_Vehicle_Weapons);
			Upgrader::getInstance()->addUpgrade(UpgradeTypes::Terran_Vehicle_Plating);
		}

		Broodwar->printf("Responding to HEAVY enemy AIR.");
	}

	if (enemyAir == LIGHT_MANY || enemyAir == LIGHT_FEW || enemyAir == MIXED_FEW) {	//respond with anti-air
		mainSquad->addSetup(UnitTypes::Terran_Goliath, 4);
		mainSquad->addSetup(UnitTypes::Terran_Wraith, 4);
		buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Missile_Turret, currentSupply));

		if (economy == STRONG || economy == ABUNDANT){

			buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Starport, currentSupply));
			buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Factory, currentSupply));
			Upgrader::getInstance()->addUpgrade(UpgradeTypes::Charon_Boosters);
		}

		Broodwar->printf("Responding to LIGHT enemy AIR.");
	}

	//responds to enemy mech
	if (enemyMech == HEAVY_MANY || enemyMech == LIGHT_MANY || enemyMech == MIXED_MANY) {
		TechManager::getInstance()->techUpTo(UnitTypes::Terran_Ghost);
		//TechManager::getInstance()->techUpTo(TechTypes::Lockdown);
		//TechManager::getInstance()->techUpTo(TechTypes::Personnel_Cloaking);

		//Broodwar->
		Upgrader::getInstance()->addTech(TechTypes::Lockdown);
		Upgrader::getInstance()->addTech(TechTypes::Personnel_Cloaking);
		
		
		mainSquad->setSetup(UnitTypes::Terran_Ghost, 12);
		//mainSquad->addSetup(UnitTypes::Terran_Medic, 3);
		
		Broodwar->printf("Responding to HEAVY enemy MECH.");
	}

	if (numberTanks < 12 && enemyLand != NONE){
		mainSquad->addSetup(UnitTypes::Terran_Siege_Tank_Tank_Mode, 4);
		//Broodwar->printf("Adding tanks to army");
	}

	if (unloadedMarines < 30){
		mainSquad->addSetup(UnitTypes::Terran_Marine, 10);
	}

	if (numberMedics < unloadedMarines / 3){
		mainSquad->setSetup(UnitTypes::Terran_Medic, unloadedMarines / 3);
	}

	if (numberGoliaths < 8){
		mainSquad->addSetup(UnitTypes::Terran_Goliath, 2);
	}

	if (numberVultures < 8 && enemyLand != NONE) {
		mainSquad->addSetup(UnitTypes::Terran_Vulture, 2);
	}

	//i want at least 1 bunker per base if economy is okay
	int desiredNumBunkers = agentManager->countNoUnits(UnitTypes::Terran_Command_Center);
	if (!inBuildPlan(UnitTypes::Terran_Bunker) && numBunkers < desiredNumBunkers && (economy == STRONG || economy == ABUNDANT) ) {
		buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Bunker, currentSupply));
	}

	//TODO: count number of buildings that produce military and decide whether to produce more
	//TODO: handle locked resources

	//demands all idle military buildings to produce units
	if (force) {
		AgentManager* agentManager = AgentManager::getInstance();
		Agentset* barracks = agentManager->getAgentsOfType(UnitTypes::Terran_Barracks);
		Agentset* factories = agentManager->getAgentsOfType(UnitTypes::Terran_Factory); 
		Agentset* starports = agentManager->getAgentsOfType(UnitTypes::Terran_Starport);

		//TODO: count minerals and gas spent to prevent depleting resources

		//handle idle barracks
		int totalMarines = agentManager->countNoUnits(UnitTypes::Terran_Marine);
		int totalFirebats = agentManager->countNoUnits(UnitTypes::Terran_Firebat);
		int totalMedics = agentManager->countNoUnits(UnitTypes::Terran_Medic);
		for (auto b : *barracks) {
			if (b->getUnit()->isIdle()) {

				//trains medics until reach 1/3 of infantry number
				if (totalMedics < (totalMarines + totalFirebats) / 3) {
					b->getUnit()->train(UnitTypes::Terran_Medic);
					totalMedics++;
				}
				//trains firebats until we have up to 1/3 of marines
				else if (totalFirebats < totalMarines / 3) {
					b->getUnit()->train(UnitTypes::Terran_Firebat);
					totalFirebats++;
				}
				else {
					b->getUnit()->train(UnitTypes::Terran_Marine);
					totalMarines++;
				}
			}
		}

		//handle idle factories, if we need anti air, train goliaths up to a certain number
		bool needAntiAir = MilitaryEvaluator::getInstance()->needAntiAir();
		int trainedGoliaths = 0;
		for (auto f : *factories) {
			if (f->getUnit()->isIdle()) {
				if (needAntiAir && trainedGoliaths < 4) {
					f->getUnit()->train(UnitTypes::Terran_Goliath);
					trainedGoliaths++;
				}
				else {
					f->getUnit()->train(UnitTypes::Terran_Siege_Tank_Tank_Mode);
				}
			}
		}

		//idle starports produce wraiths
		for (auto s : *starports) {
			if (s->getUnit()->isIdle()) {
				s->getUnit()->train(UnitTypes::Terran_Wraith);
			}
		}

		//prevent memory leaks?
		delete barracks;
		delete factories;
		delete starports;
		
	}
}

void TerranMain::improveEconomy() {
	AgentManager* agentManager = AgentManager::getInstance();
	int numWorkers = agentManager->countNoUnits(UnitTypes::Terran_SCV);
	if (numWorkers >= 60) {
		Broodwar->printf("Already with max. workers. No actions will be performed to improve economy.");
		return;
	}

	//counts number of accessible minerals
	int mineralPatches = 0;
	Agentset* bases = agentManager->getAgentsOfType(UnitTypes::Terran_Command_Center);

	for (auto base : *bases) {	//TODO: create a method for this
		Unitset closeFromBase = Broodwar->getUnitsInRadius(base->getUnit()->getPosition(), 8 * 32);	//8 tiles * 32 pixels per tile

		for (auto unit: closeFromBase) {
			if (unit->getType().isMineralField()) {
				mineralPatches++;
			}
		}
	}
	Broodwar->printf("Found %d mineral patches", mineralPatches);
	int desiredNumWorkers = 3 * mineralPatches;

	if (noWorkers >= desiredNumWorkers) {
		Broodwar->printf("NoWorkers already covers needs");
	}
	else {
		Broodwar->printf("Increasing noWorkers from %d to %d", noWorkers, desiredNumWorkers);
		noWorkers = desiredNumWorkers;
		if (noWorkers > 60) noWorkers = 60;
	}

	//checks the need for an expansion
	EconomyStrength economy = EconomyEvaluator::getInstance()->evaluateEconomy();
	int gameSeconds = Broodwar->elapsedTime();

	if ((economy == WEAK || economy == INTERMEDIATE) && gameSeconds > 600) {
		int currentSupply = Broodwar->self()->supplyUsed() / 2;
		//Broodwar->printf("Locking resources to build Cmd Center");
		//ResourceManager::getInstance()->lockResources(UnitTypes::Terran_Command_Center);
		//Constructor::getInstance()->expand(UnitTypes::Terran_Command_Center);
		buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Command_Center, currentSupply));
	}

	delete bases;	//avoid memory leak?

}

void TerranMain::techUp(){

	techInfantryUp();

	int currentSupply = Broodwar->self()->supplyUsed();

	//tests if we have barracks or it is in build order
	if (Constructor::getInstance()->needBuilding(UnitTypes::Terran_Barracks)){
		buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Barracks, currentSupply));
		//TechManager::getInstance()->techUpTo(UnitTypes::Terran_Barracks);
		return;
	}


	//basic stuff: academy, engineering bay
	if (Constructor::getInstance()->needBuilding(UnitTypes::Terran_Academy)) {
		buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Academy, currentSupply));
		//TechManager::getInstance()->techUpTo(UnitTypes::Terran_Academy);
	}
	if (Constructor::getInstance()->needBuilding(UnitTypes::Terran_Engineering_Bay)) {
		buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Engineering_Bay, currentSupply));
		//TechManager::getInstance()->techUpTo(UnitTypes::Terran_Engineering_Bay);
	}

	//if enemy is going airborne, we'd better research goliath range // wraith 
	MilitaryForce enemyAir = MilitaryEvaluator::getInstance()->evaluateEnemyAir();
	if (enemyAir == MIXED_FEW || enemyAir == MIXED_MANY || enemyAir == LIGHT_MANY || enemyAir == HEAVY_FEW || enemyAir == HEAVY_MANY){

		//if we don't have a factory and armory, build one for goliaths
		if (Constructor::getInstance()->needBuilding(UnitTypes::Terran_Factory)) {
			Broodwar->printf("Adding Factory to counter enemy air");
			buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Factory, currentSupply));
		}

		if (Constructor::getInstance()->needBuilding(UnitTypes::Terran_Armory)) {
			Broodwar->printf("Adding Armory to counter enemy air");
			buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Armory, currentSupply));
		}

		//check for goliath pre-reqs
		if (TechManager::getInstance()->preRequisitesSatisfiedFor(UnitTypes::Terran_Goliath)){
			Upgrader::getInstance()->addUpgrade(UpgradeTypes::Charon_Boosters);
			Upgrader::getInstance()->addUpgrade(UpgradeTypes::Terran_Vehicle_Weapons);
		}
		else {
			TechManager::getInstance()->techUpTo(UnitTypes::Terran_Goliath);
		}

		//if we don't have a Starport and armory, build one for Wraiths
		if (Constructor::getInstance()->needBuilding(UnitTypes::Terran_Starport)) {
			Broodwar->printf("Adding Starport to counter enemy air");
			buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Starport, currentSupply));
		}

		//check for wraith pre-reqs
		if (TechManager::getInstance()->preRequisitesSatisfiedFor(UnitTypes::Terran_Wraith)){
			Upgrader::getInstance()->addUpgrade(UpgradeTypes::Terran_Ship_Weapons);
		}
		else {
			TechManager::getInstance()->techUpTo(UnitTypes::Terran_Wraith);
		}
	}

	//if enemy is going land, we'd better research tank stuff // infantry
	MilitaryForce enemyLand = MilitaryEvaluator::getInstance()->evaluateEnemyLand();
	if (enemyLand == HEAVY_FEW || enemyLand == HEAVY_MANY || enemyLand == MIXED_FEW || enemyLand == MIXED_MANY) {
		//respond with heavy land

		//if we don't have a factory build one for tanks
		if (Constructor::getInstance()->needBuilding(UnitTypes::Terran_Factory)) {
			Broodwar->printf("Adding Factory to counter enemy land");
			buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Factory, currentSupply));
		}

		//check for tank pre-reqs
		if (TechManager::getInstance()->preRequisitesSatisfiedFor(UnitTypes::Terran_Siege_Tank_Siege_Mode)){
			Upgrader::getInstance()->addUpgrade(UpgradeTypes::Terran_Vehicle_Weapons);
		}
		else {
			Upgrader::getInstance()->addTech(TechTypes::Tank_Siege_Mode);
			//TechManager::getInstance()->techUpTo(UnitTypes::Terran_Siege_Tank_Siege_Mode);	//TODO: check if Siege Mode is being researched
		}

	}
	if (enemyLand == MIXED_FEW || enemyLand == MIXED_MANY || enemyLand == LIGHT_FEW || enemyLand == LIGHT_MANY) {
		//respond with infantry

		//check for Firebat/Medic
		if (TechManager::getInstance()->preRequisitesSatisfiedFor(UnitTypes::Terran_Medic)){
			Upgrader::getInstance()->addUpgrade(UpgradeTypes::Caduceus_Reactor);
			Upgrader::getInstance()->addUpgrade(UpgradeTypes::Terran_Infantry_Weapons);
		}
		else {
			TechManager::getInstance()->techUpTo(UnitTypes::Terran_Medic);	
		}
	}

}

void TerranMain::techInfantryUp() {
	Broodwar->printf("Teching infantry up...");
	int currentSupply = Broodwar->self()->supplyUsed() / 2;
	PlayerInterface* self = Broodwar->self();
	Upgrader* upgrader = Upgrader::getInstance();

	//counts infantry members 
	int numMarines = 0, numFirebats = 0, numMedics = 0, numGhosts = 0;
	
	for (auto s : squads) {
		numMarines += s->countMembersOfType(UnitTypes::Terran_Marine);
		numFirebats += s->countMembersOfType(UnitTypes::Terran_Firebat);
		numMedics += s->countMembersOfType(UnitTypes::Terran_Medic);
		numGhosts += s->countMembersOfType(UnitTypes::Terran_Ghost);
	}

	//if more than 5 marines, research increased range and checks need for academy
	if (numMarines > 5 && !self->isUpgrading(UpgradeTypes::U_238_Shells) && self->getUpgradeLevel(UpgradeTypes::U_238_Shells) == 0) {
		if (Constructor::getInstance()->needBuilding(UnitTypes::Terran_Academy)) {
			Broodwar->printf("Adding Academy");
			buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Academy, currentSupply));
		}
		else {
			Broodwar->printf("Adding U-238 Shells upgrade");
			upgrader->addUpgrade(UpgradeTypes::U_238_Shells);
		}
		//buildplan.push_back(BuildplanEntry(, currentSupply));
	}

	//if more than 10 infantry, upgrade weapons and armor and research Stim Pack
	if (numMarines + numFirebats + numGhosts > 10) {
		if (Constructor::getInstance()->needBuilding(UnitTypes::Terran_Engineering_Bay)) {
			Broodwar->printf("Adding Eng. Bay");
			buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Engineering_Bay, currentSupply));
		}

		else {
			Broodwar->printf("Adding infantry weapons/armor upgrades");
			upgrader->addUpgrade(UpgradeTypes::Terran_Infantry_Weapons);
			upgrader->addUpgrade(UpgradeTypes::Terran_Infantry_Armor);
		}
		if (!self->isResearching(TechTypes::Stim_Packs) && !self->hasResearched(TechTypes::Stim_Packs)) {
			Broodwar->printf("Adding Stim Packs research");
			upgrader->addTech(TechTypes::Stim_Packs);
		}
	}

	//if have ghosts, research lockdown and cloack
	if (numGhosts > 3) {
		if (!self->isResearching(TechTypes::Personnel_Cloaking) && !self->hasResearched(TechTypes::Personnel_Cloaking)) {
			Broodwar->printf("Adding Personnel Cloaking research");
			upgrader->addTech(TechTypes::Personnel_Cloaking);
		}

		if (!self->isResearching(TechTypes::Lockdown) && !self->hasResearched(TechTypes::Lockdown)) {
			upgrader->addTech(TechTypes::Lockdown);
		}

	}
}

void TerranMain::techMechanicUp() {

}

void TerranMain::doUpgrades(){
	Broodwar->printf("I don't know how to upgrade up yet :(");
}

void TerranMain::expand(){
	Constructor* constructor = Constructor::getInstance();
	/*
	//skips adding expansion if it's already in the plan
	if (constructor->containsType(UnitTypes::Terran_Command_Center) || constructor->isBeingBuilt(UnitTypes::Terran_Command_Center)){
		return;
	}*/
	for (size_t i = 0; i < buildplan.size(); i++) {
		if (buildplan.at(i).unittype == UnitTypes::Terran_Command_Center) {
			return;
		}

	}

	//conditions satisfied, will expand (hopefully)
	//constructor->expand(UnitTypes::Terran_Command_Center);
	int currentSupply = Broodwar->self()->supplyUsed() / 2;
	buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Command_Center, currentSupply));

	//int currentSupply = Broodwar->self()->supplyUsed() / 2;

	//locks resources to allow expansion
	//buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Command_Center, currentSupply));
	
	//ResourceManager::getInstance()->lockResources(UnitTypes::Terran_Command_Center);
}