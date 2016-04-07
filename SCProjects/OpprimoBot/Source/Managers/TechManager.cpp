#include <algorithm> 
#include "TechManager.h"
#include "AgentManager.h"
#include "Constructor.h"
#include "BuildplanEntry.h"
#include "../Commander/Commander.h"

using namespace std;

TechManager* TechManager::instance = NULL;

TechManager::TechManager() : buildPlan(Commander::getInstance()->getBuildPlan()) {
	lastCallFrame = 0;
	debug = false;
	agentManager = AgentManager::getInstance();
	//buildPlan = Commander::getInstance()->getBuildPlan();
}


TechManager::~TechManager() {
}

TechManager* TechManager::getInstance(){
	if (instance == NULL) {
		instance = new TechManager();
	}
	return instance;
}

void TechManager::toggleDebug(){
	debug = !debug;
	Broodwar->printf("TechManager debug set to %s", debug);
}

void TechManager::printInfo(){
	Broodwar->drawBoxScreen(488, 90, 602, 200, Colors::Black, true);
	Broodwar->drawTextScreen(490, 90, "\x03Tech Tree");
	Broodwar->drawLineScreen(490, 106, 600, 106, Colors::Orange);

	stringstream ss;
	for (auto a : unitJobs)	{
		ss << a.first.c_str() << "|" << a.second << endl;
	}
	Broodwar->drawTextScreen(490, 106, ss.str().c_str());
}

void TechManager::computeActions(){
	if (Broodwar->getFrameCount() - lastCallFrame < 12) return;	//no need to execute too often

	lastCallFrame = Broodwar->getFrameCount();

	for (auto target : unitJobs)	{	
		if (preRequisitesSatisfiedFor(target.first)){
			unitJobs.emplace(target.first, ACCOMPLISHED);
			break;
		}

		//recursively finds the first unsatisfied pre-requisite:
		map< UnitType, int > preRequisites = target.first.requiredUnits();
		for (auto preReq : preRequisites){
			//if pre-requisite of pre-requisite is not satisfied, tech up to it
			if (!preRequisitesSatisfiedFor(preReq.first)){
				techUpTo(preReq.first); 
				break;
			}

			//tests need of purchasing this pre-requisite (does not exist and is not in build plan)
			if (agentManager->countNoUnits(preReq.first) == 0 && Constructor::getInstance()->noInProduction(preReq.first) == 0 && !inBuildPlan(preReq.first)){
				buildPlan.push_back(BuildplanEntry(preReq.first, Broodwar->self()->supplyUsed() / 2));
				Broodwar->printf("Teching up! Adding %s to build plan.", preReq.first.c_str());
				unitJobs.emplace(target.first, IN_PROGRESS);
			}
			
			//pre-requisite is on the way
			else {
				unitJobs.emplace(target.first, IN_PROGRESS);
			}
		}


	}
}

bool TechManager::inBuildPlan(UnitType type){
	for (auto bpEntry : buildPlan){
		if (bpEntry.type == BuildplanEntry::BUILDING && bpEntry.unittype == type){
			return true;
		}
	}
	return false;
}

bool TechManager::preRequisitesSatisfiedFor(UnitType type){
	
	//checks whether necessary Tech was researched
	if (!Broodwar->self()->hasResearched(type.requiredTech())){
		return false;
	}

	//checks whether necessary UnitTypes exist in required ammount
	map< UnitType, int > preRequisites = type.requiredUnits();
	for (auto preReq : preRequisites){
		if (agentManager->countNoUnits(preReq.first) < preReq.second){
			return false;
		}
	}
	return true; //no pre-requisites were violated
}

bool TechManager::preRequisitesSatisfiedFor(TechType type){
	//check if i have researched this already
	if (Broodwar->self()->hasResearched(type)){
		return true;
	}

	//check whether necessary buildings exist
	if (! AgentManager::getInstance()->hasBuilding(type.whatResearches())){
		return false;
	}
	return true;	//no pre-requisites were violated
}

bool TechManager::preRequisitesSatisfiedFor(UpgradeType type, int level){
	//check if i have researched this already
	if (Broodwar->self()->getUpgradeLevel(type) == level){
		return true;
	}

	//check whether necessary buildings exist
	if (! AgentManager::getInstance()->hasBuilding(type.whatUpgrades() )){
		return false;
	}

	//check additional requirements
	if (AgentManager::getInstance()->countNoFinishedUnits(type.whatsRequired()) == 0){
		return false;
	}

	return true;	//no pre-requisites were violated
}

/** Advances the technology tree to be able to build a given unit */
void TechManager::techUpTo(UnitType type){
	unitJobs.emplace(type, NOT_STARTED);
}

/** Advances the technology tree to be able to research a given tech */
void TechManager::techUpTo(TechType type){
	techJobs.emplace(type, NOT_STARTED);
}

/** Advances the technology tree to be able to research a given upgrade in a given level */
void TechManager::techUpTo(UpgradeType type, int level){
	pair<UpgradeType, int> *job = new pair<UpgradeType, int>(type, level);
	upgradeJobs.emplace(make_pair(type, level), NOT_STARTED);
}