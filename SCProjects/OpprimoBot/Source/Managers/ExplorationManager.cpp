#include "ExplorationManager.h"
#include "BuildingPlacer.h"

ExplorationManager* ExplorationManager::instance = NULL;

ExplorationManager::ExplorationManager()
{
	//Add the regions for this map
	for(BWTA::Region* r : getRegions())
	{
		explore.insert(new RegionItem(r));
	}

	siteSetFrame = 0;

	lastCallFrame = Broodwar->getFrameCount();

	expansionSite = TilePosition(-1, -1);
}

ExplorationManager::~ExplorationManager()
{
	instance = NULL;
}

ExplorationManager* ExplorationManager::getInstance()
{
	if (instance == NULL)
	{
		instance = new ExplorationManager();
	}
	return instance;
}

void ExplorationManager::computeActions()
{
	//Dont call too often
	int cFrame = Broodwar->getFrameCount();
	if (cFrame - lastCallFrame < 9)
	{
		return;
	}
	lastCallFrame = cFrame;

	for (auto &a : explore)
	{
		if (Broodwar->isVisible(a->location))
		{
			a->frameVisited = Broodwar->getFrameCount();
		}
	}

	for (auto &unit : Broodwar->enemy()->getUnits()){
		if (unit->isVisible()){
			addSpottedUnit(unit);
		}
	}
}

TilePosition ExplorationManager::searchExpansionSite()
{
	getExpansionSite();

	if (expansionSite.x == -1)
	{
		expansionSite = BuildingPlacer::getInstance()->findExpansionSite();
		siteSetFrame = Broodwar->getFrameCount();
	}

	return expansionSite;
}

TilePosition ExplorationManager::getExpansionSite()
{
	if (expansionSite.x >= 0)
	{
		if (Broodwar->getFrameCount() - siteSetFrame > 500)
		{
			expansionSite = TilePosition(-1, -1);
		}
	}

	return expansionSite;
}

void ExplorationManager::setExplored(TilePosition pos)
{
	for (auto &a : explore)
	{
		if (a->location.x == pos.x && a->location.y == pos.y)
		{
			a->frameVisited = Broodwar->getFrameCount();
			return;
		}
	}

	Broodwar << "Cannot set to explored" << endl;
}

void ExplorationManager::setExpansionSite(TilePosition pos)
{
	if (pos.x >= 0)
	{
		siteSetFrame = Broodwar->getFrameCount();
		expansionSite = pos;
	}
}

TilePosition ExplorationManager::getNextToExplore(Squad* squad)
{
	int longestVisitFrame = Broodwar->getFrameCount();
	TilePosition nextPos = TilePosition(-1, -1);
	for (auto &a : explore)
	{
		if (a->frameVisited < longestVisitFrame)
		{
			longestVisitFrame = a->frameVisited;
			nextPos = a->location;
		}
	}
	return nextPos;
}

void ExplorationManager::printInfo() {
	//Uncomment this if you want to draw a mark at detected enemy buildings.
	for (auto enemyBldg : enemyBuildings) {
		//if (enemyBldg-> isActive()){
			int x1 = enemyBldg->getTilePosition().x * 32;
			int y1 = enemyBldg->getTilePosition().y * 32;
			int x2 = x1 + 32;
			int y2 = y1 + 32;

			Broodwar->drawBoxMap(x1,y1,x2,y2,Colors::Blue,true);
		//}
	}

	for (auto enemyUnit : enemyUnits) {
		//if (enemyBldg-> isActive()){
		int x1 = enemyUnit->getTilePosition().x * 32;
		int y1 = enemyUnit->getTilePosition().y * 32;
		int x2 = x1 + 32;
		int y2 = y1 + 32;

		Broodwar->drawBoxMap(x1, y1, x2, y2, Colors::Grey, true);
		//}
	}

	//Draw a circle around detectors
}

void ExplorationManager::addSpottedUnit(Unit unit) {
	if (unit->getType().isBuilding()) {
		//Check if we already have seen this building
		bool found = false;
		for (auto &a : enemyBuildings) {
			if (a->getUnitID() == unit->getID()) {
				found = true;
				break;
			}
		}

		if (!found) {
			enemyBuildings.insert(new SpottedObject(unit));
		}
	}
	else {	//not a building
		bool found = false;
		for (auto &a : enemyUnits) {
			if (a->getUnitID() == unit->getID()) {
				found = true;
				a->update(unit); //updates information from the recently seen unit
				break;
			}
		}

		if (!found) {
			enemyUnits.insert(new SpottedObject(unit));
		}
	}
}

void ExplorationManager::unitDestroyed(Unit unit) {
	TilePosition uPos = unit->getTilePosition();

	if (unit->getType().isBuilding()) {
		bool removed = false;
		for (auto &a : enemyBuildings) {
			if (a->isAt(unit->getTilePosition())) {
				enemyBuildings.erase(a);
				return;
			}
		}
	}
	else {	//not a building
		for (auto &a : enemyUnits) {
			if (a->getUnitID() == unit->getID()) {
				enemyUnits.erase(a);
				return;
			}
		}
	}
}

void ExplorationManager::cleanup()
{
	bool cont = true;

	while (cont)
	{
		cont = false;
		for (auto &a : enemyBuildings)
		{
			if (Broodwar->isVisible(a->getTilePosition()))
			{
				bool found = false;
				for (auto &u : Broodwar->enemy()->getUnits())
				{
					if (u->exists())
					{
						if (u->getID() == a->getUnitID())
						{
							found = true;
							break;
						}
					}
				}
				if (!found)
				{
					enemyBuildings.erase(a);
					cont = true;
					break;
				}
			}
		}
	}
}

int ExplorationManager::getSpottedInfluenceInRegion(const BWTA::Region* region)
{
	int im = 0;
	for (auto &a : enemyBuildings)
	{
		if (region->getPolygon().isInside(a->getPosition()))
		{
			im += a->getType().buildScore();
		}
	}
	return im;
}

TilePosition ExplorationManager::getClosestSpottedBuilding(TilePosition start)
{
	cleanup();

	TilePosition pos = TilePosition(-1, -1);
	double bestDist = 100000;

	for (auto &a : enemyBuildings)
	{
		double cDist = start.getDistance(a->getTilePosition());
		if (cDist < bestDist)
		{
			bestDist = cDist;
			pos = a->getTilePosition();
		}
	}

	return pos;
}

bool ExplorationManager::canReach(TilePosition a, TilePosition b)
{
	BWTA::Region* ra = BWTA::getRegion(a);
	BWTA::Region* rb = BWTA::getRegion(b);
	return (ra->isReachable(rb));
}

bool ExplorationManager::canReach(BaseAgent* agent, TilePosition b)
{
	return agent->getUnit()->hasPath(Position(b));
}

bool ExplorationManager::enemyIsProtoss()
{
	for (auto &u : Broodwar->getPlayers())
	{
		if (u->isEnemy(Broodwar->self()))
		{
			if (u->getRace().getID() == Races::Protoss.getID())
			{
				return true;
			}
		}
	}
	return false;
}

bool ExplorationManager::enemyIsZerg()
{
	for (auto &u : Broodwar->getPlayers())
	{
		if (u->isEnemy(Broodwar->self()))
		{
			if (u->getRace().getID() == Races::Zerg.getID())
			{
				return true;
			}
		}
	}
	return false;
}

bool ExplorationManager::enemyIsTerran()
{
	for (auto &u : Broodwar->getPlayers())
	{
		if (u->isEnemy(Broodwar->self()))
		{
			if (u->getRace().getID() == Races::Terran.getID())
			{
				return true;
			}
		}
	}
	return false;
}

bool ExplorationManager::enemyIsUnknown()
{
	if (!enemyIsTerran() && !enemyIsProtoss() && !enemyIsZerg())
	{
		return true;
	}
	return false;
}

