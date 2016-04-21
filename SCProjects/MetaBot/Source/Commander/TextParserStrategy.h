#pragma once

#include "Terran/TerranMain.h"

class TextParserStrategy : public TerranMain {
public:
    TextParserStrategy();
    ~TextParserStrategy();

    /** Returns the unique id for this strategy. */
    static string getStrategyId() {
        return "Text Parser Strategy";
    }
};

