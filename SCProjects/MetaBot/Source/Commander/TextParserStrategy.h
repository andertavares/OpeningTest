#pragma once

#include "Terran/TerranMain.h"

class TextParserStrategy : public TerranMain {
	static string strategyID;

public:
    TextParserStrategy();
    ~TextParserStrategy();

    /** Returns the unique id for this strategy. */
    static string getStrategyId() {
		return strategyID;
    }
};

