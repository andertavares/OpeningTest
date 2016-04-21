#include "MetaBot.h"
#include "Managers/Constructor.h"
#include "Managers/ExplorationManager.h"
#include "Managers\TechManager.h"
#include "Influencemap/MapManager.h"
#include "Managers/BuildingPlacer.h"
#include "Commander/Commander.h"
#include "Pathfinding/Pathfinder.h"
#include "Managers/Upgrader.h"
#include "Managers/ResourceManager.h"
#include "Utils/Profiler.h"
#include "Pathfinding/NavigationAgent.h"
#include "Utils/Config.h"
#include "Commander/StrategySelector.h"
#include "Utils/Statistics.h"
#include "Data/MatchData.h"
#include "Data\Configuration.h"
#include "Managers/AgentManager.h"
#include <Shlwapi.h>

using namespace BWAPI;

bool analyzed;
bool analysis_just_finished;
bool leader = false;

void MetaBot::onStart()
{
	//Enable/disable file writing stuff
	Profiler::getInstance()->disable();
	Statistics::getInstance()->enable();
	StrategySelector::getInstance()->enable();

	Profiler::getInstance()->start("OnInit");

	MatchData::getInstance()->registerMatchBegin();

	//Needed for text commands to work
	Broodwar->enableFlag(Flag::UserInput);

	//Uncomment to enable complete map information
	//Broodwar->enableFlag(Flag::CompleteMapInformation);

	//Analyze map using BWTA
	BWTA::readMap();
	analyzed = false;
	analysis_just_finished = false;
	//CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)AnalyzeThread, NULL, 0, NULL); //Threaded version
	AnalyzeThread();

	profile = false;

	//Init our singleton agents
	AgentManager::getInstance();
	BuildingPlacer::getInstance();
	Constructor::getInstance();
	Upgrader::getInstance();
	TechManager::getInstance();
	ResourceManager::getInstance();
	Pathfinder::getInstance();

	//Fill pathfinder
	for (BaseLocation* base : getBaseLocations())
	{
		TilePosition pos = base->getTilePosition();
		Pathfinder::getInstance()->requestPath(Broodwar->self()->getStartLocation(), pos);
	}

	MapManager::getInstance();
	
	//Add the units we have from start to agent manager
	for (auto &u : Broodwar->self()->getUnits())
	{
		AgentManager::getInstance()->addAgent(u);
	}

	running = true;

	Broodwar->setCommandOptimizationLevel(2); //0--3

	//Debug mode. Active panels.
	Commander::getInstance()->toggleSquadsDebug();
	Commander::getInstance()->toggleBuildplanDebug();
	Upgrader::getInstance()->toggleDebug();
	loop.toggleUnitDebug();
	//loop.toggleBPDebug();
	//End Debug mode

	//Set speed
	speed = Configuration::getInstance()->speed;
	Broodwar->printf("Setting speed to %d.", speed);
	Broodwar->setLocalSpeed(speed);

	//set gui
	bool gui = Configuration::getInstance()->enableGUI;
	Broodwar->printf("Setting GUI to %s.", gui ? "enabled" : "disabled");
	Broodwar->setGUI(gui);	

	Profiler::getInstance()->end("OnInit");
}

void MetaBot::gameStopped()
{
	Pathfinder::getInstance()->stop();
	Profiler::getInstance()->dumpToFile();
	running = false;

	//Delete singletons
	delete Pathfinder::getInstance();
	delete Profiler::getInstance();
	delete AgentManager::getInstance();
	delete BuildingPlacer::getInstance();
	delete ResourceManager::getInstance();
	delete Constructor::getInstance();
	delete Commander::getInstance();
	delete ExplorationManager::getInstance();
	delete NavigationAgent::getInstance();
	delete StrategySelector::getInstance();
	delete MapManager::getInstance();
	delete MatchData::getInstance();
	delete Configuration::getInstance();
}

void MetaBot::onEnd(bool isWinner)
{
	if (Broodwar->elapsedTime() / 60 < 4) return;

	int win = 0;
	if (isWinner) win = 1;
	if (Broodwar->elapsedTime() / 60 >= 80) win = 2;

	StrategySelector::getInstance()->addResult(win);
	StrategySelector::getInstance()->saveStats();
	Statistics::getInstance()->saveResult(win);

	MatchData::getInstance()->registerMatchFinish(win);
	MatchData::getInstance()->writeResult();

	gameStopped();
}

void MetaBot::onFrame()
{
	Profiler::getInstance()->start("OnFrame");

	if (!running)
	{
		//Game over. Do nothing.
		return;
	}
	if (!Broodwar->isInGame())
	{
		//Not in game. Do nothing.
		gameStopped();
		return;
	}
	if (Broodwar->isReplay())
	{
		//Replay. Do nothing.
		return;
	}

	if (Broodwar->elapsedTime() / 60 >= 81)
	{
		//Stalled game. Leave it.
		Broodwar->leaveGame();
		return;
	}

	loop.computeActions();
	loop.show_debug();

	Config::getInstance()->displayBotName();

	Profiler::getInstance()->end("OnFrame");
}

void MetaBot::onSendText(std::string text)
{
	if (text == "a") {
		Commander::getInstance()->forceAttack();
	}
	else if (text == "p") {
		profile = !profile;
	}
	else if (text == "d") {
		loop.toggleDebug();
	}
	else if (text == "pf" && NavigationAgent::pathfinding_version == 2) {
		loop.togglePFDebug();
	}
	else if (text == "bp") {
		loop.toggleBPDebug();
	}
	else if (text == "spf") {
		NavigationAgent::pathfinding_version++;
		if (NavigationAgent::pathfinding_version > 2) NavigationAgent::pathfinding_version = 0;
	}
	else if (text.substr(0, 2) == "sq")
	{
		if (text == "sq")
		{
			loop.setDebugSQ(-1);
		}
		else
		{
			int id = atoi(&text[2]);
			loop.setDebugSQ(id);
		}
	}
	else if (text == "+")
	{
		speed -= 4;
		if (speed < 0)
		{
			speed = 0;
		}
		Broodwar << "Changed game speed: " << speed << endl;
		Broodwar->setLocalSpeed(speed);
	}
	else if (text == "++")
	{
		speed = 0;
		Broodwar << "Changed game speed: " << speed << endl;
		Broodwar->setLocalSpeed(speed);
	}
	else if (text == "-")
	{
		speed += 4;
		Broodwar << "Changed game speed: " << speed << endl;
		Broodwar->setLocalSpeed(speed);
	}
	else if (text == "--")
	{
		speed = 24;
		Broodwar << "Changed game speed: " << speed << endl;
		Broodwar->setLocalSpeed(speed);
	}
	else if (text == "t") {
		Upgrader::getInstance()->toggleDebug();
		TechManager::getInstance()->toggleDebug();
	}
	
	else if (text == "s")
	{
		Commander::getInstance()->toggleSquadsDebug();
	}
	else if (text == "b")
	{
		Commander::getInstance()->toggleBuildplanDebug();
	}
	else if (text == "i")
	{
		loop.toggleUnitDebug();
	}
	else
	{
		Broodwar << "You typed '" << text << "'!" << endl;
	}
}

void MetaBot::onReceiveText(BWAPI::Player player, std::string text)
{
	Broodwar << player->getName() << " said '" << text << "'" << endl;
}

void MetaBot::onPlayerLeft(BWAPI::Player player)
{

}

void MetaBot::onNukeDetect(BWAPI::Position target)
{
	if (target != Positions::Unknown)
	{
		TilePosition t = TilePosition(target);
		Broodwar << "Nuclear Launch Detected at (" << t.x << "," << t.y << ")" << endl;
	}
	else
	{
		Broodwar << "Nuclear Launch Detected" << endl;
	}
}

void MetaBot::onUnitDiscover(BWAPI::Unit unit)
{
	if (Broodwar->isReplay() || Broodwar->getFrameCount() <= 1) return;

	if (unit->getPlayer()->getID() != Broodwar->self()->getID())
	{
		if (!unit->getPlayer()->isNeutral() && !unit->getPlayer()->isAlly(Broodwar->self()))
		{
			ExplorationManager::getInstance()->addSpottedUnit(unit);
		}
	}
}

void MetaBot::onUnitEvade(BWAPI::Unit unit)
{

}

void MetaBot::onUnitShow(BWAPI::Unit unit)
{
	if (Broodwar->isReplay() || Broodwar->getFrameCount() <= 1) return;

	if (unit->getPlayer()->getID() != Broodwar->self()->getID())
	{
		if (!unit->getPlayer()->isNeutral() && !unit->getPlayer()->isAlly(Broodwar->self()))
		{
			ExplorationManager::getInstance()->addSpottedUnit(unit);
		}
	}
}

void MetaBot::onUnitHide(BWAPI::Unit unit)
{

}

void MetaBot::onUnitCreate(BWAPI::Unit unit)
{
	if (unit->getPlayer()->getID() == Broodwar->self()->getID())
	{
		loop.addUnit(unit);
	}
}

void MetaBot::onUnitComplete(BWAPI::Unit unit)
{
	
}

void MetaBot::onUnitDestroy(BWAPI::Unit unit)
{
	if (Broodwar->isReplay() || Broodwar->getFrameCount() <= 1) return;

	loop.unitDestroyed(unit);
}

void MetaBot::onUnitMorph(BWAPI::Unit unit)
{
	if (Broodwar->isReplay() || Broodwar->getFrameCount() <= 1) return;

	if (unit->getPlayer()->getID() == Broodwar->self()->getID())
	{
		if (Constructor::isZerg())
		{
			loop.morphUnit(unit);
		}
		else
		{
			loop.addUnit(unit);
		}
	}
}

void MetaBot::onUnitRenegade(BWAPI::Unit unit)
{

}

void MetaBot::onSaveGame(std::string gameName)
{
	Broodwar << "The game was saved to '" << gameName << "'" << endl;
}

DWORD WINAPI AnalyzeThread()
{
	BWTA::analyze();

	analyzed = true;
	analysis_just_finished = true;
	return 0;
}

bool BotTournamentModule::onAction(int actionType, void *parameter)
{
	switch (actionType)
	{
	case Tournament::SendText:
	case Tournament::Printf:
		return true;
	case Tournament::EnableFlag:
		switch (*(int*)parameter)
		{
		case Flag::CompleteMapInformation:
		case Flag::UserInput:
			// Disallow these two flags
			return false;
		}
		// Allow other flags if we add more that don't affect gameplay specifically
		return true;
	case Tournament::LeaveGame:
	case Tournament::PauseGame:
	case Tournament::ResumeGame:
	case Tournament::SetFrameSkip:
	case Tournament::SetGUI:
	case Tournament::SetLocalSpeed:
	case Tournament::SetMap:
		return false; // Disallow these actions
	case Tournament::SetLatCom:
	case Tournament::SetTextSize:
		return true; // Allow these actions
	case Tournament::SetCommandOptimizationLevel:
		return *(int*)parameter > MINIMUM_COMMAND_OPTIMIZATION; // Set a minimum command optimization level 
		// to reduce APM with no action loss
	default:
		break;
	}
	return true;
}

void BotTournamentModule::onFirstAdvertisement()
{
	leader = true;
	Broodwar->sendText("Welcome to " TOURNAMENT_NAME "!");
	Broodwar->sendText("Brought to you by " SPONSORS ".");
}
