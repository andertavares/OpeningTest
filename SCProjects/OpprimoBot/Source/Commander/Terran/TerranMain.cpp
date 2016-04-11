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
	if (buildplan.size() > 0 && Broodwar->elapsedTime() < 60 * 10) return;	//skips doing anything if buildplan is not satisfied before 10 minutes
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

		enhanceMilitary();
		techUp();
		doUpgrades();
	}
	else if (economyStrength == STRONG){
		Broodwar->printf("Strong economy :-)");
		expand();
		enhanceMilitary();
		techUp();
	}
	else if (economyStrength == INTERMEDIATE){
		Broodwar->printf("Intermediate economy :-|");
		//expand or military up
		enhanceMilitary();
	}
	else if (economyStrength == WEAK){
		Broodwar->printf("Weak economy :(");
		//militaryUp
		enhanceMilitary();
	}

	
}

void TerranMain::enhanceMilitary(){
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
}

void TerranMain::techUp(){
	//tests if we have barracks or it is in build order
	if (!AgentManager::getInstance()->countNoUnits(UnitTypes::Terran_Barracks) && !Constructor::getInstance()->containsType(UnitTypes::Terran_Barracks)){
		TechManager::getInstance()->techUpTo(UnitTypes::Terran_Barracks);
		return;
	}


	//basic stuff: academy, engineering bay
	if (!AgentManager::getInstance()->countNoUnits(UnitTypes::Terran_Academy) && !Constructor::getInstance()->containsType(UnitTypes::Terran_Academy)){
		TechManager::getInstance()->techUpTo(UnitTypes::Terran_Academy);
	}
	if (!AgentManager::getInstance()->countNoUnits(UnitTypes::Terran_Engineering_Bay) && !Constructor::getInstance()->containsType(UnitTypes::Terran_Engineering_Bay)){
		TechManager::getInstance()->techUpTo(UnitTypes::Terran_Engineering_Bay);
	}



	//if enemy is going airborne, we'd better research goliath range // wraith 
	MilitaryForce enemyAir = MilitaryEvaluator::getInstance()->evaluateEnemyAir();
	if (enemyAir == MIXED_FEW || enemyAir == MIXED_MANY || enemyAir == LIGHT_MANY || enemyAir == HEAVY_FEW || enemyAir == HEAVY_MANY){

		//check for goliath pre-reqs
		if (TechManager::getInstance()->preRequisitesSatisfiedFor(UnitTypes::Terran_Goliath)){
			Upgrader::getInstance()->addUpgrade(UpgradeTypes::Charon_Boosters);
			Upgrader::getInstance()->addUpgrade(UpgradeTypes::Terran_Vehicle_Weapons);
		}
		else {
			TechManager::getInstance()->techUpTo(UnitTypes::Terran_Goliath);
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
		//check for tank pre-reqs
		if (TechManager::getInstance()->preRequisitesSatisfiedFor(UnitTypes::Terran_Siege_Tank_Siege_Mode)){
			Upgrader::getInstance()->addUpgrade(UpgradeTypes::Terran_Vehicle_Weapons);
		}
		else {
			TechManager::getInstance()->techUpTo(UnitTypes::Terran_Siege_Tank_Siege_Mode);	//TODO: check if Siege Mode is being researched
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

void TerranMain::doUpgrades(){
	Broodwar->printf("I don't know how to upgrade up yet :(");
}

void TerranMain::expand(){
	Constructor* constructor = Constructor::getInstance();
	
	//skips adding expansion if it's already in the plan
	if (constructor->containsType(UnitTypes::Terran_Command_Center) || constructor->isBeingBuilt(UnitTypes::Terran_Command_Center)){
		return;
	}
	for (size_t i = 0; i < buildplan.size(); i++) {
		if (buildplan.at(i).unittype == UnitTypes::Terran_Command_Center) {
			return;
		}

	}

	//conditions satisfied, will expand (hopefully)
	constructor->expand(UnitTypes::Terran_Command_Center);

	//int currentSupply = Broodwar->self()->supplyUsed() / 2;

	//locks resources to allow expansion
	//buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Command_Center, currentSupply));
	
	//ResourceManager::getInstance()->lockResources(UnitTypes::Terran_Command_Center);
}