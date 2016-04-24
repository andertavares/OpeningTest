#include "TextParserStrategy.h"
#include "../Utils/ParseUtils.h"
#include "../Data/Configuration.h"
#include "../Commander/StrategySelector.h"

//initializes ID to a default name
string TextParserStrategy::strategyID = "Text Parser Strategy";

TextParserStrategy::TextParserStrategy() {

    strategyID = ParseUtils::ParseOpeningFile(
		Configuration::OPENINGS_DIR + StrategySelector::getInstance()->getStrategyID(), buildplan
	);

}


TextParserStrategy::~TextParserStrategy() {}
