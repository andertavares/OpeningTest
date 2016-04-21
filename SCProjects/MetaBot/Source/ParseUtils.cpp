#include "ParseUtils.h"

#include <vector>

void ParseUtils::ParseConfigFile(const std::string & filename, std::vector<BuildplanEntry>& buildplan) {
    rapidjson::Document doc;
    BWAPI::Race race = BWAPI::Races::Terran;

    std::string text;
    std::ifstream inputFile(filename);
    if (inputFile.is_open()) {
        std::string line; /* or other suitable maximum line size */
        do {
            getline(inputFile, line);
            text = text + line + "\n";
        } while (inputFile);

        inputFile.close();
    }
    else {
        BWAPI::Broodwar->printf("Could not open file: %s", filename.c_str());
    }

    if (text.length() == 0) {
        return;
    }

    if (doc.HasMember("Build name") && doc["Build name"].IsObject()) {
        const rapidjson::Value &info = doc["Build name"];
        std::vector<char*> commandTypes{
            "CommandCenter",
            "ComsatStation",
            "NuclearSilo",
            "SupplyDepot",
            "Barracks",
            "Refinery",
            "EngineeringBay",
            "Bunker",
            "Academy",
            "MissileTurret",
            "Factory",
            "MachineShop",
            "Starport",
            "ControlTower",
            "Armory",
            "ScienceFacility",
            "PhysicsLab",
            "CovertOps"
        };

        for (const auto& commandType : commandTypes) {
            std::vector<int> atSupplies;
            JSONTools::ReadArray(commandType, info, atSupplies);

            BWAPI::UnitType unittype;
            BWAPI::TechType techtype;
            BWAPI::UpgradeType upgradetype;

            for (const auto& atSupply : atSupplies) {
                int type = ParseType(commandType, unittype, techtype, upgradetype);

                if (type == UnitCode) {
                    buildplan.push_back(BuildplanEntry(unittype, atSupply));
                }
                else if (type == UpgradeCode) {
                    buildplan.push_back(BuildplanEntry(upgradetype, atSupply));
                }
                else if (type == TechCode) {
                    buildplan.push_back(BuildplanEntry(techtype, atSupply));
                }
            }
        }
    }
}


int ParseUtils::ParseType(const std::string& commandType, UnitType& unitype, TechType& techtype, UpgradeType& upgradetype) {
    if (commandType == "CommandCenter") {
        unitype = BWAPI::UnitTypes::Terran_Command_Center;
        return ParseUtils::UnitCode;
    }
    else if (commandType == "ComsatStation") {
        unitype = BWAPI::UnitTypes::Terran_Comsat_Station;
        return ParseUtils::UnitCode;
    }
    else if (commandType == "NuclearSilo") {
        unitype = BWAPI::UnitTypes::Terran_Nuclear_Silo;
        return ParseUtils::UnitCode;
    }
    else if (commandType == "SupplyDepot") {
        unitype = BWAPI::UnitTypes::Terran_Supply_Depot;
        return ParseUtils::UnitCode;
    }
    else if (commandType == "Barracks") {
        unitype = BWAPI::UnitTypes::Terran_Barracks;
        return ParseUtils::UnitCode;
    }
    else if (commandType == "Refinery") {
        unitype = BWAPI::UnitTypes::Terran_Refinery;
        return ParseUtils::UnitCode;
    }
    else if (commandType == "EngineeringBay") {
        unitype = BWAPI::UnitTypes::Terran_Engineering_Bay;
        return ParseUtils::UnitCode;
    }
    else if (commandType == "Bunker") {
        unitype = BWAPI::UnitTypes::Terran_Bunker;
        return ParseUtils::UnitCode;
    }
    else if (commandType == "Academy") {
        unitype = BWAPI::UnitTypes::Terran_Academy;
        return ParseUtils::UnitCode;
    }
    else if (commandType == "MissileTurret") {
        unitype = BWAPI::UnitTypes::Terran_Missile_Turret;
        return ParseUtils::UnitCode;
    }
    else if (commandType == "Factory") {
        unitype = BWAPI::UnitTypes::Terran_Factory;
        return ParseUtils::UnitCode;
    }
    else if (commandType == "MachineShop") {
        unitype = BWAPI::UnitTypes::Terran_Machine_Shop;
        return ParseUtils::UnitCode;
    }
    else if (commandType == "Starport") {
        unitype = BWAPI::UnitTypes::Terran_Starport;
        return ParseUtils::UnitCode;
    }
    else if (commandType == "ControlTower") {
        unitype = BWAPI::UnitTypes::Terran_Control_Tower;
        return ParseUtils::UnitCode;
    }
    else if (commandType == "Armory") {
        unitype = BWAPI::UnitTypes::Terran_Armory;
        return ParseUtils::UnitCode;
    }
    else if (commandType == "ScienceFacility") {
        unitype = BWAPI::UnitTypes::Terran_Science_Facility;
        return ParseUtils::UnitCode;
    }
    else if (commandType == "PhysicsLab") {
        unitype = BWAPI::UnitTypes::Terran_Physics_Lab;
        return ParseUtils::UnitCode;
    }
    else if (commandType == "CovertOps") {
        unitype = BWAPI::UnitTypes::Terran_Covert_Ops;
        return ParseUtils::UnitCode;
    }
    else if (commandType == "StimPacks") {
        techtype = BWAPI::TechTypes::Stim_Packs;
        return ParseUtils::TechCode;
    }
    else if (commandType == "PersonnelCloaking") {
        techtype = BWAPI::TechTypes::Personnel_Cloaking;
        return ParseUtils::TechCode;
    }
    else if (commandType == "Lockdown") {
        techtype = BWAPI::TechTypes::Lockdown;
        return ParseUtils::TechCode;
    }
    else if (commandType == "TankSiegeMode") {
        techtype = BWAPI::TechTypes::Tank_Siege_Mode;
        return ParseUtils::TechCode;
    }
    else if (commandType == "EMPShockwave") {
        techtype = BWAPI::TechTypes::EMP_Shockwave;
        return ParseUtils::TechCode;
    }
    else if (commandType == "YamatoGun") {
        techtype = BWAPI::TechTypes::Yamato_Gun;
        return ParseUtils::TechCode;
    }
    else if (commandType == "TerranInfantryArmor") {
        upgradetype = BWAPI::UpgradeTypes::Terran_Infantry_Armor;
        return ParseUtils::UpgradeCode;
    }
    else if (commandType == "TerranInfantryWeapons") {
        upgradetype = BWAPI::UpgradeTypes::Terran_Infantry_Weapons;
        return ParseUtils::UpgradeCode;
    }
    else if (commandType == "TerranShipPlating") {
        upgradetype = BWAPI::UpgradeTypes::Terran_Ship_Plating;
        return ParseUtils::UpgradeCode;
    }
    else if (commandType == "TerranShipWeapons") {
        upgradetype = BWAPI::UpgradeTypes::Terran_Ship_Weapons;
        return ParseUtils::UpgradeCode;
    }
    else if (commandType == "TerranVehiclePlating") {
        upgradetype = BWAPI::UpgradeTypes::Terran_Vehicle_Plating;
        return ParseUtils::UpgradeCode;
    }
    else if (commandType == "TerranVehicleWeapons") {
        upgradetype = BWAPI::UpgradeTypes::Terran_Vehicle_Weapons;
        return ParseUtils::UpgradeCode;
    }
    else if (commandType == "CharonBoosters") {
        upgradetype = BWAPI::UpgradeTypes::Charon_Boosters;
        return ParseUtils::UpgradeCode;
    }
    else if (commandType == "CaduceusReactor") {
        upgradetype = BWAPI::UpgradeTypes::Caduceus_Reactor;
        return ParseUtils::UpgradeCode;
    }
    else if (commandType == "U238Shells") {
        upgradetype = BWAPI::UpgradeTypes::U_238_Shells;
        return ParseUtils::UpgradeCode;
    }

    return -1;
}