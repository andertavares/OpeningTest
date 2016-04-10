#include "MilitaryEvaluator.h"
#include "../Managers/AgentManager.h"

MilitaryEvaluator* MilitaryEvaluator::instance = NULL;

MilitaryEvaluator::MilitaryEvaluator() {
	explorationManager = ExplorationManager::getInstance();
}

MilitaryEvaluator::~MilitaryEvaluator() {
	instance = NULL;
}

MilitaryEvaluator* MilitaryEvaluator::getInstance() {
	if (instance == NULL) {
		instance = new MilitaryEvaluator();
	}
	return instance;
}



MilitaryForce MilitaryEvaluator::evaluateEnemyAir(){
	int numHeavyUnits = 0;	//accounts for Battlecruisers, Carriers, Guardians
	int numLightUnits = 0;	//accounts for Mutalisks, Wraiths, Scouts
	SpottedObjectSet& enemyUnits = explorationManager->getSpottedUnits();
	
	numHeavyUnits = enemyUnits.countUnitsOfType(UnitTypes::Terran_Battlecruiser)
		+ enemyUnits.countUnitsOfType(UnitTypes::Protoss_Carrier)
		+ enemyUnits.countUnitsOfType(UnitTypes::Zerg_Guardian)
		+ enemyUnits.countUnitsOfType(UnitTypes::Protoss_Arbiter);

	numLightUnits = enemyUnits.countUnitsOfType(UnitTypes::Zerg_Mutalisk)
		+ enemyUnits.countUnitsOfType(UnitTypes::Terran_Wraith)
		+ enemyUnits.countUnitsOfType(UnitTypes::Protoss_Scout)
		+ enemyUnits.countUnitsOfType(UnitTypes::Protoss_Corsair);

	if (numLightUnits > 0 || numHeavyUnits > 0) {	//enemy has some air force, won't return NONE
		if (numHeavyUnits > 5 && numLightUnits > 10) {
			return MIXED_MANY;
		}
		else if (numHeavyUnits < 5 && numLightUnits < 10){
			return MIXED_FEW;
		}
		else if (numHeavyUnits > 5 && numLightUnits < 10){
			return HEAVY_MANY;
		}
		else if (numHeavyUnits < 5 && numLightUnits < 3){
			return HEAVY_FEW;
		}
		else if (numHeavyUnits < 5 && numLightUnits > 10){
			return LIGHT_MANY;
		}
		else if (numHeavyUnits < 2 && numLightUnits < 10){
			return LIGHT_FEW;
		}
		/*//won't bother returning MIXED_MANY because HEAVY_MANY is already troublesome
		if (numHeavyUnits > 5) {
			return HEAVY_MANY;
		}
		else if (numLightUnits > 10 && numHeavyUnits < 3){
			return LIGHT_MANY;
		}
		else if (numLightUnits < 10 && numHeavyUnits < 3){
			return LIGHT_FEW;
		}
		else if (numLightUnits > 0 && numHeavyUnits > 0){
			return MIXED_FEW;
		}
		else return HEAVY_FEW;*/
	}

	return NONE;
}

MilitaryForce MilitaryEvaluator::evaluateEnemyMech(){
	int numHeavyUnits = 0;	//accounts for Battlecruisers, Carriers, Guardians
	int numLightUnits = 0;	//accounts for Mutalisks, Wraiths, Scouts
	SpottedObjectSet& enemyUnits = explorationManager->getSpottedUnits();

	numHeavyUnits = enemyUnits.countUnitsOfType(UnitTypes::Terran_Battlecruiser)
		+ enemyUnits.countUnitsOfType(UnitTypes::Terran_Siege_Tank_Siege_Mode)
		+ enemyUnits.countUnitsOfType(UnitTypes::Terran_Siege_Tank_Tank_Mode)
		+ enemyUnits.countUnitsOfType(UnitTypes::Protoss_Carrier)
		+ enemyUnits.countUnitsOfType(UnitTypes::Protoss_Reaver)
		+ enemyUnits.countUnitsOfType(UnitTypes::Protoss_Arbiter);

	numLightUnits = enemyUnits.countUnitsOfType(UnitTypes::Terran_Vulture)
		+ enemyUnits.countUnitsOfType(UnitTypes::Terran_Science_Vessel)
		+ enemyUnits.countUnitsOfType(UnitTypes::Terran_Wraith)
		+ enemyUnits.countUnitsOfType(UnitTypes::Terran_Goliath)
		+ enemyUnits.countUnitsOfType(UnitTypes::Protoss_Scout)
		+ enemyUnits.countUnitsOfType(UnitTypes::Protoss_Corsair);

	if (numLightUnits > 0 || numHeavyUnits > 0) {	//enemy has some mech, won't return NONE

		//won't bother returning MIXED_MANY because HEAVY_MANY is already troublesome
		if (numHeavyUnits > 5) {
			return HEAVY_MANY;
		}
		else if (numLightUnits > 10 && numHeavyUnits < 3){
			return LIGHT_MANY;
		}
		else if (numLightUnits < 10 && numHeavyUnits < 3){
			return LIGHT_FEW;
		}
		else if (numLightUnits > 0 && numHeavyUnits > 0){
			return MIXED_FEW;
		}
		else return HEAVY_FEW;
	}

	return NONE;
}

MilitaryForce MilitaryEvaluator::evaluateEnemyLand(){
	int numHeavyUnits = 0;	//accounts for Infantry, Lings, Hydras, Zealots, Goons, High Templars, DTs
	int numLightUnits = 0;	//accounts for Tanks, Ultralisks, Reavers, Archon, defensive buildings
	SpottedObjectSet& enemyUnits = explorationManager->getSpottedUnits();
	SpottedObjectSet& enemyBldgs = explorationManager->getSpottedBuildings();

	numLightUnits = enemyUnits.countUnitsOfType(UnitTypes::Terran_Marine)
		+ enemyUnits.countUnitsOfType(UnitTypes::Terran_Firebat)
		+ enemyUnits.countUnitsOfType(UnitTypes::Terran_Ghost)
		+ enemyUnits.countUnitsOfType(UnitTypes::Terran_Goliath)
		+ enemyUnits.countUnitsOfType(UnitTypes::Terran_Vulture)
		+ enemyUnits.countUnitsOfType(UnitTypes::Zerg_Zergling)
		+ enemyUnits.countUnitsOfType(UnitTypes::Zerg_Hydralisk)
		+ enemyUnits.countUnitsOfType(UnitTypes::Protoss_Zealot)
		+ enemyUnits.countUnitsOfType(UnitTypes::Protoss_Dragoon)
		+ enemyUnits.countUnitsOfType(UnitTypes::Protoss_Dark_Templar)
		+ enemyUnits.countUnitsOfType(UnitTypes::Protoss_High_Templar);

	 numHeavyUnits = enemyUnits.countUnitsOfType(UnitTypes::Terran_Siege_Tank_Siege_Mode)
		+ enemyUnits.countUnitsOfType(UnitTypes::Terran_Siege_Tank_Tank_Mode)
		+ enemyUnits.countUnitsOfType(UnitTypes::Zerg_Ultralisk)
		+ enemyUnits.countUnitsOfType(UnitTypes::Protoss_Reaver)
		+ enemyUnits.countUnitsOfType(UnitTypes::Protoss_Archon)
		+ 2 * enemyUnits.countUnitsOfType(UnitTypes::Zerg_Sunken_Colony)		//defensive buildings count more
		+ 2 * enemyUnits.countUnitsOfType(UnitTypes::Protoss_Photon_Cannon)
		+ 2 * enemyUnits.countUnitsOfType(UnitTypes::Terran_Bunker);

	if (numLightUnits > 0 || numHeavyUnits > 0) {	//enemy has some land force, won't return NONE

		if (numHeavyUnits > 5 && numLightUnits > 10) {
			return MIXED_MANY;
		}
		else if (numHeavyUnits < 5 && numLightUnits < 10){
			return MIXED_FEW;
		}
		else if (numHeavyUnits > 5 && numLightUnits < 10){
			return HEAVY_MANY;
		}
		else if (numHeavyUnits < 5 && numLightUnits < 3){
			return HEAVY_FEW;
		}
		else if (numHeavyUnits < 5 && numLightUnits > 10){
			return LIGHT_MANY;
		}
		else if (numHeavyUnits < 2 && numLightUnits < 10){
			return LIGHT_FEW;
		}

		/*
		else if (numLightUnits > 10 && numHeavyUnits < 3){
			return LIGHT_MANY;
		}
		else if (numLightUnits < 10 && numHeavyUnits < 3){
			return LIGHT_FEW;
		}
		else if (numLightUnits > 0 && numHeavyUnits > 0){
			return MIXED_FEW;
		}
		else return HEAVY_FEW;
		*/
	}

	return NONE;
}

MilitaryForce MilitaryEvaluator::evaluateOurForces(){
	AgentManager* agentManager = AgentManager::getInstance();

	//accounts for heavy land and air units
	int numHeavyUnits = agentManager->countNoFinishedUnits(UnitTypes::Terran_Siege_Tank_Siege_Mode)
		+ agentManager->countNoFinishedUnits(UnitTypes::Terran_Siege_Tank_Tank_Mode)
		+ agentManager->countNoFinishedUnits(UnitTypes::Zerg_Ultralisk)
		+ agentManager->countNoFinishedUnits(UnitTypes::Protoss_Reaver)
		+ agentManager->countNoFinishedUnits(UnitTypes::Protoss_Archon)
		
		+ agentManager->countNoFinishedUnits(UnitTypes::Terran_Battlecruiser)
		+ agentManager->countNoFinishedUnits(UnitTypes::Protoss_Carrier)
		+ agentManager->countNoFinishedUnits(UnitTypes::Zerg_Guardian);;

	//accounts for light land and air units
	int numLightUnits = agentManager->countUnloadedUnits(UnitTypes::Terran_Marine)
		+ agentManager->countUnloadedUnits(UnitTypes::Terran_Firebat)
		+ agentManager->countUnloadedUnits(UnitTypes::Terran_Ghost)
		+ agentManager->countNoFinishedUnits(UnitTypes::Terran_Goliath)
		+ agentManager->countNoFinishedUnits(UnitTypes::Terran_Vulture)
		+ agentManager->countNoFinishedUnits(UnitTypes::Zerg_Zergling)
		+ agentManager->countNoFinishedUnits(UnitTypes::Zerg_Hydralisk)
		+ agentManager->countNoFinishedUnits(UnitTypes::Protoss_Zealot)
		+ agentManager->countNoFinishedUnits(UnitTypes::Protoss_Dragoon)
		+ agentManager->countNoFinishedUnits(UnitTypes::Protoss_Dark_Templar)
		+ agentManager->countNoFinishedUnits(UnitTypes::Protoss_High_Templar)
		
		+ agentManager->countNoFinishedUnits(UnitTypes::Zerg_Mutalisk)
		+ agentManager->countNoFinishedUnits(UnitTypes::Terran_Wraith)
		+ agentManager->countNoFinishedUnits(UnitTypes::Protoss_Scout)
		+ agentManager->countNoFinishedUnits(UnitTypes::Protoss_Corsair);


	if (numLightUnits > 5 || numHeavyUnits > 5) {	//i have some forces, won't return NONE
		if (numHeavyUnits > 8 && numLightUnits > 15) {
			return MIXED_MANY;
		}
		else if (numHeavyUnits < 8 && numLightUnits < 15) {
			return MIXED_FEW;
		}
		else if (numHeavyUnits > 8 && numLightUnits < 10) {
			return HEAVY_MANY;
		}
		else if (numHeavyUnits < 8 && numLightUnits < 5) {
			return HEAVY_FEW;
		}
		else if (numHeavyUnits < 8 && numLightUnits > 15) {
			return LIGHT_MANY;
		}
		else if (numHeavyUnits < 5 && numLightUnits < 15) {
			return LIGHT_FEW;
		}


		//won't bother returning MIXED_MANY because HEAVY_MANY is already troublesome
		/*if (numHeavyUnits > 10) {
			return HEAVY_MANY;
		}
		else if (numLightUnits > 30){
			if (numHeavyUnits > 7) return MIXED_MANY;
			else return LIGHT_MANY;
		}
		else if (numLightUnits < 10) {
			if (numHeavyUnits < 3) return LIGHT_FEW;
			else return MIXED_FEW;
		}
		else if (numLightUnits > 5 && numHeavyUnits > 0){
			return MIXED_FEW;
		}
		else return HEAVY_FEW;*/
	}

	return NONE;
}

string MilitaryEvaluator::stringOf(MilitaryForce force){
	if (force == NONE) return "None";
	if (force == LIGHT_FEW) return "Light, few";
	if (force == LIGHT_MANY) return "Light, many";
	if (force == HEAVY_FEW) return "Heavy, few";
	if (force == HEAVY_MANY) return "Heavy, many";
	if (force == MIXED_FEW) return "Mixed, few";
	if (force == MIXED_MANY) return "Mixed, many";
	else return "";
}