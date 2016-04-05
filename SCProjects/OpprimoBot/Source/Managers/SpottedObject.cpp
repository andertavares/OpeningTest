#include "SpottedObject.h"

SpottedObject::SpottedObject(Unit mUnit)
{
	type = mUnit->getType();
	position = mUnit->getPosition();
	tilePosition = mUnit->getTilePosition();
	unitID = mUnit->getID();
}

void SpottedObject::update(Unit mUnit){

	if (unitID != mUnit->getID()) {
		Broodwar->printf("Warning, attempted updating units with diff IDs (expected=%d, received=%d)", unitID, mUnit->getID());
		return;
	}

	type = mUnit->getType();
	position = mUnit->getPosition();
	tilePosition = mUnit->getTilePosition();
	
}

int SpottedObject::getUnitID()
{
	return unitID;
}

UnitType SpottedObject::getType()
{
	return type;
}

Position SpottedObject::getPosition()
{
	return position;
}

TilePosition SpottedObject::getTilePosition()
{
	return tilePosition;
}

bool SpottedObject::isAt(TilePosition tilePos)
{
	if (tilePos.x == tilePosition.x && tilePos.y == tilePosition.y)
	{
		return true;
	}
	return false;
}


