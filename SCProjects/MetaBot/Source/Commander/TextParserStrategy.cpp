#include "TextParserStrategy.h"
#include "../ParseUtils.h"


TextParserStrategy::TextParserStrategy() {

    ParseUtils::ParseConfigFile("../../config.txt", buildplan);

}


TextParserStrategy::~TextParserStrategy() {}
