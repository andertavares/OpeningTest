#ifndef __TECHMANAGER_H__
#define __TECHMANAGER_H__

#include <BWAPI.h>
#include <vector>
#include "BuildplanEntry.h"
#include "AgentManager.h"

using namespace std;
using namespace BWAPI;

enum Status {
	/** Not building or researching pre-requisite */
	NOT_STARTED,

	/** Building or researching pre-requisite */
	IN_PROGRESS,

	/** Already built or researched pre-requisite */
	ACCOMPLISHED
};

class TechManager {
private:
	static TechManager* instance;

	int lastCallFrame;
	bool debug;
	AgentManager* agentManager;
	vector<BuildplanEntry>& buildPlan;
	map<UnitType, Status> unitJobs;		
	map<pair<UpgradeType, int>, Status> upgradeJobs;	//maps upgrade and level to status
	map<TechType, Status> techJobs;
	
	TechManager();

	/** Returns whether the given unit type is present in the build plan */
	bool inBuildPlan(UnitType type);

public:
	static TechManager* getInstance();
	~TechManager();

	/** Performs actions when teching up */
	void computeActions();

	/** Switches between printing debug info on screen or not */
	void toggleDebug();

	/** Writes information on screen */
	void printInfo();

	/** Returns the Building that trains or researches the given Type */
	UnitType getPrerequisiteFor(UnitType type);
	
	/** Returns the Building that trains or researches the given Type */
	UnitType getPrerequisiteFor(UpgradeType type);
	
	/** Returns the Building that trains or researches the given Type */
	UnitType getPrerequisiteFor(TechType type);

	/** Returns whether needed buildings exists and required Techs are researched */
	bool preRequisitesSatisfiedFor(UnitType type);

	/** Returns whether needed buildings exists and required Techs are researched */
	bool preRequisitesSatisfiedFor(UpgradeType type, int level);

	/** Returns whether needed buildings exists and required Techs are researched */
	bool preRequisitesSatisfiedFor(TechType type);

	/** Advances the technology tree to be able to build a given unit */
	void techUpTo(UnitType type);

	/** Advances the technology tree to be able to research a given tech */
	void techUpTo(TechType type);

	/** Advances the technology tree to be able to research a given upgrade in a given level */
	void techUpTo(UpgradeType type, int level);

	/** Returns a string representation of the status of a Tech job */
	static string stringOf(Status status);


};

#endif