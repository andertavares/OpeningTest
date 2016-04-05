#include "EconomyEvaluator.h"

EconomyEvaluator* EconomyEvaluator::instance = NULL;

EconomyEvaluator::EconomyEvaluator() {
	
}

EconomyEvaluator::~EconomyEvaluator() {
	instance = NULL;
}

EconomyEvaluator* EconomyEvaluator::getInstance() {
	if (instance == NULL) {
		instance = new EconomyEvaluator();
	}
	return instance;
}

ResourceLevels EconomyEvaluator::evaluateMinerals(){
	int minerals = Broodwar->self()->minerals();

	if (minerals < 100)  return LOW;
	if (minerals < Broodwar->self()->getRace().getSupplyProvider().mineralPrice()) return INTERMEDIATE;
	if (minerals < 1000) return HIGH;
	return THOUSANDS;
}

ResourceLevels EconomyEvaluator::evaluateGas(){
	int gas = Broodwar->self()->gas();

	if (gas < 100)  return LOW;
	if (gas < Broodwar->self()->getRace().getSupplyProvider().mineralPrice()) return INTERMEDIATE;
	if (gas < 1000) return HIGH;
	return THOUSANDS;
}

EconomyStrength EconomyEvaluator::evaluateEconomy() {
	ResourceLevels mineralLevel = evaluateMinerals();
	ResourceLevels gasLevel = evaluateMinerals();

	if (mineralLevel == LOW || gasLevel == LOW)  return WEAK;
	if (mineralLevel == INTERMEDIATE || gasLevel == INTERMEDIATE)  return FAIR;
	if (mineralLevel == HIGH || gasLevel == HIGH) return STRONG;
	return ABUNDANT;
}
