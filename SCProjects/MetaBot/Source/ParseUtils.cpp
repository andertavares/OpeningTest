#include "ParseUtils.h"

#include <vector>

using namespace std;
using namespace BWAPI;

void ParseUtils::ParseConfigFile(const string & filename, vector<BuildplanEntry>& buildplan) {
    rapidjson::Document doc;
    Race race = Races::Terran;

    string text;
    ifstream inputFile(filename);
    if (inputFile.is_open()) {
        std::string line; /* or other suitable maximum line size */
        do {
            getline(inputFile, line);
            text = text + line + "\n";
        } while (inputFile);

        inputFile.close();
    }
    else {
        Broodwar->printf("Could not open file: %s", filename.c_str());
    }

    bool parsingFailed = doc.Parse(text.c_str()).HasParseError();
    if (parsingFailed) {
        Broodwar->printf("Parsing Failed");
        return;
    }

    if (text.length() == 0) {
        return;
    }

    if (doc.HasMember("Build name") && doc["Build name"].IsObject()) {
        const rapidjson::Value &info = doc["Build name"];
        std::vector<char*> commandTypes{
            "Command Center",
            "Comsat Station",
            "Nuclear Silo",
            "Supply Depot",
            "Barracks",
            "Refinery",
            "Engineering Bay",
            "Bunker",
            "Academy",
            "Missile Turret",
            "Factory",
            "Machine Shop",
            "Starport",
            "Control Tower",
            "Armory",
            "Science Facility",
            "Physics Lab",
            "Covert Ops",
            "StimPacks",
            "Personnel Cloaking",
            "Lockdown",
            "Tank Siege Mode",
            "EMP Shockwave",
            "Yamato Gun",
            "Terran Infantry Armor",
            "Terran Infantry Weapons",
            "Terran Ship Plating",
            "Terran Ship Weapons",
            "Terran Vehicle Plating",
            "Terran Vehicle Weapons",
            "Charon Boosters",
            "Caduceus Reactor",
            "U238 Shells"
        };

        for (const auto& commandType : commandTypes) {
            std::vector<int> atSupplies;
            JSONTools::ReadArray(commandType, info, atSupplies);

            UnitType unittype;
            TechType techtype;
            UpgradeType upgradetype;

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
    if (commandType == "Command Center") {
        unitype = UnitTypes::Terran_Command_Center;
        return ParseUtils::UnitCode;
    }
    else if (commandType == "Comsat Station") {
        unitype = UnitTypes::Terran_Comsat_Station;
        return ParseUtils::UnitCode;
    }
    else if (commandType == "Nuclear Silo") {
        unitype = UnitTypes::Terran_Nuclear_Silo;
        return ParseUtils::UnitCode;
    }
    else if (commandType == "Supply Depot") {
        unitype = UnitTypes::Terran_Supply_Depot;
        return ParseUtils::UnitCode;
    }
    else if (commandType == "Barracks") {
        unitype = UnitTypes::Terran_Barracks;
        return ParseUtils::UnitCode;
    }
    else if (commandType == "Refinery") {
        unitype = UnitTypes::Terran_Refinery;
        return ParseUtils::UnitCode;
    }
    else if (commandType == "Engineering Bay") {
        unitype = UnitTypes::Terran_Engineering_Bay;
        return ParseUtils::UnitCode;
    }
    else if (commandType == "Bunker") {
        unitype = UnitTypes::Terran_Bunker;
        return ParseUtils::UnitCode;
    }
    else if (commandType == "Academy") {
        unitype = UnitTypes::Terran_Academy;
        return ParseUtils::UnitCode;
    }
    else if (commandType == "Missile Turret") {
        unitype = UnitTypes::Terran_Missile_Turret;
        return ParseUtils::UnitCode;
    }
    else if (commandType == "Factory") {
        unitype = UnitTypes::Terran_Factory;
        return ParseUtils::UnitCode;
    }
    else if (commandType == "Machine Shop") {
        unitype = UnitTypes::Terran_Machine_Shop;
        return ParseUtils::UnitCode;
    }
    else if (commandType == "Starport") {
        unitype = UnitTypes::Terran_Starport;
        return ParseUtils::UnitCode;
    }
    else if (commandType == "Control Tower") {
        unitype = UnitTypes::Terran_Control_Tower;
        return ParseUtils::UnitCode;
    }
    else if (commandType == "Armory") {
        unitype = UnitTypes::Terran_Armory;
        return ParseUtils::UnitCode;
    }
    else if (commandType == "Science Facility") {
        unitype = UnitTypes::Terran_Science_Facility;
        return ParseUtils::UnitCode;
    }
    else if (commandType == "Physics Lab") {
        unitype = UnitTypes::Terran_Physics_Lab;
        return ParseUtils::UnitCode;
    }
    else if (commandType == "Covert Ops") {
        unitype = UnitTypes::Terran_Covert_Ops;
        return ParseUtils::UnitCode;
    }
    else if (commandType == "Stim Packs") {
        techtype = TechTypes::Stim_Packs;
        return ParseUtils::TechCode;
    }
    else if (commandType == "Personnel Cloaking") {
        techtype = TechTypes::Personnel_Cloaking;
        return ParseUtils::TechCode;
    }
    else if (commandType == "Lockdown") {
        techtype = TechTypes::Lockdown;
        return ParseUtils::TechCode;
    }
    else if (commandType == "Tank Siege Mode") {
        techtype = TechTypes::Tank_Siege_Mode;
        return ParseUtils::TechCode;
    }
    else if (commandType == "EMP Shockwave") {
        techtype = TechTypes::EMP_Shockwave;
        return ParseUtils::TechCode;
    }
    else if (commandType == "Yamato Gun") {
        techtype = TechTypes::Yamato_Gun;
        return ParseUtils::TechCode;
    }
    else if (commandType == "Terran Infantry Armor") {
        upgradetype = UpgradeTypes::Terran_Infantry_Armor;
        return ParseUtils::UpgradeCode;
    }
    else if (commandType == "Terran Infantry Weapons") {
        upgradetype = UpgradeTypes::Terran_Infantry_Weapons;
        return ParseUtils::UpgradeCode;
    }
    else if (commandType == "Terran Ship Plating") {
        upgradetype = UpgradeTypes::Terran_Ship_Plating;
        return ParseUtils::UpgradeCode;
    }
    else if (commandType == "Terran Ship Weapons") {
        upgradetype = UpgradeTypes::Terran_Ship_Weapons;
        return ParseUtils::UpgradeCode;
    }
    else if (commandType == "Terran Vehicle Plating") {
        upgradetype = UpgradeTypes::Terran_Vehicle_Plating;
        return ParseUtils::UpgradeCode;
    }
    else if (commandType == "Terran Vehicle Weapons") {
        upgradetype = UpgradeTypes::Terran_Vehicle_Weapons;
        return ParseUtils::UpgradeCode;
    }
    else if (commandType == "Charon Boosters") {
        upgradetype = UpgradeTypes::Charon_Boosters;
        return ParseUtils::UpgradeCode;
    }
    else if (commandType == "Caduceus Reactor") {
        upgradetype = UpgradeTypes::Caduceus_Reactor;
        return ParseUtils::UpgradeCode;
    }
    else if (commandType == "U238 Shells") {
        upgradetype = UpgradeTypes::U_238_Shells;
        return ParseUtils::UpgradeCode;
    }

    return -1;
}