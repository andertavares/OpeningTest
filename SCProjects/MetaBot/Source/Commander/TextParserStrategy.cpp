#include "TextParserStrategy.h"
#include "../ParseUtils.h"
#include "../Data/Configuration.h"


TextParserStrategy::TextParserStrategy() {

    ParseUtils::ParseConfigFile(Configuration::INPUT_DIR + "config.txt", buildplan);

}


TextParserStrategy::~TextParserStrategy() {}
