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



MilitaryWeight MilitaryEvaluator::evaluateEnemyAir(){
	return NONE;
}

MilitaryWeight MilitaryEvaluator::evaluateEnemyLand(){
	//

	return NONE;
}