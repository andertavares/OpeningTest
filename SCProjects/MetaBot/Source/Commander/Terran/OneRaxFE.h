#ifndef __ONERAXFE_H__
#define __ONERAXFE_H__

#include "TerranMain.h"
class OneRaxFE : public TerranMain {
public:
	OneRaxFE();
	~OneRaxFE();

	/** Returns the unique id for this strategy. */
	static string getStrategyId() {
		return "1 Rax FE";
	}
};

#endif
