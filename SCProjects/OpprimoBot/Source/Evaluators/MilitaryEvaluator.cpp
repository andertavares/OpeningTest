#include "MilitaryEvaluator.h"

MilitaryEvaluator* MilitaryEvaluator::instance = NULL;

MilitaryEvaluator::MilitaryEvaluator() {

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



MilitaryWeight MilitaryEvaluator::evaluateAirCombat(){
	return NONE;
}

MilitaryWeight MilitaryEvaluator::evaluateLandCombat(){
	//

	return NONE;
}