#include "MilitaryEvaluator.h"

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
		+ enemyUnits.countUnitsOfType(UnitTypes::Zerg_Guardian);

	numLightUnits = enemyUnits.countUnitsOfType(UnitTypes::Zerg_Mutalisk)
		+ enemyUnits.countUnitsOfType(UnitTypes::Terran_Wraith)
		+ enemyUnits.countUnitsOfType(UnitTypes::Protoss_Scout)
		+ enemyUnits.countUnitsOfType(UnitTypes::Protoss_Corsair);

	if (numLightUnits > 0 || numHeavyUnits > 0) {	//enemy has some air force, won't return NONE

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
	int numLightUnits = 0;	//accounts for Tanks, Ultralisks, Reavers, Archon
	SpottedObjectSet& enemyUnits = explorationManager->getSpottedUnits();

	numHeavyUnits = enemyUnits.countUnitsOfType(UnitTypes::Terran_Marine)
		+ enemyUnits.countUnitsOfType(UnitTypes::Terran_Firebat)
		+ enemyUnits.countUnitsOfType(UnitTypes::Terran_Ghost)
		+ enemyUnits.countUnitsOfType(UnitTypes::Zerg_Zergling)
		+ enemyUnits.countUnitsOfType(UnitTypes::Zerg_Hydralisk)
		+ enemyUnits.countUnitsOfType(UnitTypes::Protoss_Zealot)
		+ enemyUnits.countUnitsOfType(UnitTypes::Protoss_Dragoon)
		+ enemyUnits.countUnitsOfType(UnitTypes::Protoss_Dark_Templar)
		+ enemyUnits.countUnitsOfType(UnitTypes::Protoss_High_Templar);

	numLightUnits = enemyUnits.countUnitsOfType(UnitTypes::Terran_Siege_Tank_Siege_Mode)
		+ enemyUnits.countUnitsOfType(UnitTypes::Terran_Siege_Tank_Tank_Mode)
		+ enemyUnits.countUnitsOfType(UnitTypes::Zerg_Ultralisk)
		+ enemyUnits.countUnitsOfType(UnitTypes::Protoss_Reaver)
		+ enemyUnits.countUnitsOfType(UnitTypes::Protoss_Archon);

	if (numLightUnits > 0 || numHeavyUnits > 0) {	//enemy has some land force, won't return NONE

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