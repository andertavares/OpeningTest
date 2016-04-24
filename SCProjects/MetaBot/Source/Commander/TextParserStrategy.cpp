#include "TextParserStrategy.h"
#include "../Utils/ParseUtils.h"
#include "../Data/Configuration.h"

//initializes ID to a default name
string TextParserStrategy::strategyID = "Text Parser Strategy";

TextParserStrategy::TextParserStrategy() {

    strategyID = ParseUtils::ParseOpeningFile(
		Configuration::OPENINGS_DIR + Configuration::getInstance()->buildOrderID, buildplan
	);

}


TextParserStrategy::~TextParserStrategy() {}
