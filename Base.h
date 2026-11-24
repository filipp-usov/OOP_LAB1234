#pragma once
#include "Observer.h"
#include "UnitFactory.h"
#include "Building.h"
#include <vector>
#include <memory>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <ostream>
#include "LogSystem.h"

class Base : public IObserver {
private:
    int x, y;
    int health;
    int maxUnits;
    int currentUnits;
    int defenseLevel;
    int wood, metal, energy, gold, leather;
    std::vector<std::shared_ptr<Unit>> units; 
    std::vector<std::unique_ptr<Building>> buildings;
    int nextUnitId;
    int nextBuildingId; 
    UnitFactory* factory;
    std::string currentResearch; 
    int researchTimeLeft;
    int turnCount;

public:
    Base(int startX, int startY, int hp, int maxU, int def, int startRes, UnitFactory* f) 
        : x(startX), y(startY), health(hp), maxUnits(maxU), currentUnits(0), 
          defenseLevel(def), wood(startRes), metal(startRes), energy(startRes), 
          gold(startRes), leather(startRes), nextUnitId(1), nextBuildingId(1), factory(f), researchTimeLeft(0), turnCount(1) {}

    int getX() const { return x; }
    int getY() const { return y; }

    const std::vector<std::unique_ptr<Building>>& getBuildings() const { return buildings; }

    void createUnit(const std::string& type, int unitX, int unitY) {
        if (currentUnits >= maxUnits) { std::cout << "Base: Unit limit reached!\n"; return; }
        if (std::abs(unitX - x) > 3 || std::abs(unitY - y) > 3) {
            std::cout << "[Base] Error: Unit must be created near the base at (" << x << "," << y << ")!\n";
            return;
        }
        Unit* rawUnit = factory->createUnit(type, nextUnitId++, unitX, unitY);
        if (rawUnit) {
            rawUnit->subscribe(this);
            units.push_back(std::shared_ptr<Unit>(rawUnit));
            currentUnits++;
            std::cout << "Base: Created unit " << type << " (ID: " << rawUnit->getId() << ") at (" << unitX << "," << unitY << ")\n";
            Logger().log(LogEvent("INFO", "BASE", "base: created unit " + type + " (ID=" + std::to_string(rawUnit->getId()) + ")"));
        } else {
            std::cout << "[Base] Factory failed to create " << type << "!\n";
        }
    }

    void buildBuilding(const std::string& type, int bx, int by) {
        if (wood < 50 || metal < 30 || gold < 20) {
            std::cout << "[Base] Not enough resources! (Need 50 Wood, 30 Metal, 20 Gold)\n";
            return;
        }
        if (bx == x && by == y) { std::cout << "[Base] Cannot build on the base itself!\n"; return; }
        
        wood -= 50; metal -= 30; gold -= 20;
        buildings.push_back(std::make_unique<Building>(nextBuildingId++, type, 100, bx, by)); 
        std::cout << "[Base] Started building " << type << " (ID: " << nextBuildingId - 1 << ") at (" << bx << "," << by << ")\n";
        Logger().log(LogEvent("INFO", "BASE", "base: building " + type + " started at (" + std::to_string(bx) + "," + std::to_string(by) + ")"));
    }

    void produceUnit(const std::string& type, int buildingId) {
        for (auto& b : buildings) {
            if (b->id == buildingId) {
                if (b->isUnderConstruction) {
                    std::cout << "[Base] Building is still under construction!\n";
                    return;
                }
                if (b->type != "Barracks" && b->type != "Factory") {
                    std::cout << "[Base] This building cannot produce units!\n";
                    return;
                }
                if (gold < 20) { std::cout << "[Base] Not enough gold to produce unit!\n"; return; }
                gold -= 20;

                Unit* rawUnit = factory->createUnit(type, nextUnitId++, b->x + 1, b->y); 
                if (rawUnit) {
                    rawUnit->subscribe(this);
                    units.push_back(std::shared_ptr<Unit>(rawUnit));
                    currentUnits++;
                    std::cout << "[Base] Produced " << type << " at (" << b->x + 1 << "," << b->y << ")!\n";
                    Logger().log(LogEvent("INFO", "BASE", "base: produced unit " + type + " in building ID=" + std::to_string(buildingId)));
                }
                return;
            }
        }
        std::cout << "[Base] Building with ID " << buildingId << " not found!\n";
    }

    void startResearch(const std::string& tech) {
        if (energy < 20 || gold < 50) { std::cout << "[Base] Not enough resources! (Need 20 Energy, 50 Gold)\n"; return; }
        energy -= 20; gold -= 50;
        currentResearch = tech;
        researchTimeLeft = 3; 
        std::cout << "[Base] Research started: " << tech << " (Takes 3 turns)\n";
        Logger().log(LogEvent("INFO", "BASE", "base: research '" + tech + "' started"));
    }

    void distributeResources(const std::string& resType, int amount) {
        if (resType == "wood") wood += amount;
        else if (resType == "metal") metal += amount;
        else if (resType == "energy") energy += amount;
        else if (resType == "gold") gold += amount;
        else if (resType == "leather") leather += amount;
        std::cout << "[Base] Distributed " << amount << " " << resType << ".\n";
    }

    void gatherResources() {
        wood += 10; metal += 5; leather += 2;
        std::cout << "[Base] Unit gathered: +10 Wood, +5 Metal, +2 Leather.\n";
        Logger().log(LogEvent("INFO", "BASE", "base: gathered resources (+10 wood, +5 metal, +2 leather)"));
    }

    void tick() { 
        turnCount++;
        std::cout << "\n--- Turn " << turnCount << " ---\n";
        for (auto& b : buildings) b->progressBuild();
        if (researchTimeLeft > 0) {
            researchTimeLeft--;
            if (researchTimeLeft == 0) {
                std::cout << "[Base] Research completed: " << currentResearch << "!\n";
            }
        }
    }

    void printStats() {
        std::cout << "\n--- BASE STATS (Turn " << turnCount << ") ---\n";
        std::cout << "Resources: Wood: " << wood << " | Metal: " << metal 
                  << " | Energy: " << energy << " | Gold: " << gold << " | Leather: " << leather << "\n";
        std::cout << "Units (" << currentUnits << "/" << maxUnits << "):\n";
        for (auto& u : units) {
            std::cout << "  ID: " << u->getId() << " | " << u->getType() 
                      << " | HP: " << u->getHealth().getValue() 
                      << " | Armor: " << u->getArmor().getValue() 
                      << " | Dmg: " << u->getAttack().getValue()
                      << " | Pos: (" << u->getX() << "," << u->getY() << ")\n";
        }
        std::cout << "Buildings:\n";
        for (auto& b : buildings) {
            std::cout << "  ID: " << b->id << " | " << b->type << " at (" << b->x << "," << b->y << ")"
                      << " | Status: " << (b->isUnderConstruction ? "Building" : "Done") 
                      << " (" << b->buildProgress << "%)\n";
        }
        if (researchTimeLeft > 0) 
            std::cout << "Research: " << currentResearch << " (Time left: " << researchTimeLeft << " turns)\n";
        else 
            std::cout << "No active research.\n";
    }

    void onUnitEvent(const UnitEvent& event) override {
        if (event.type == EventType::Destroyed) {
            std::cout << "[Observer] Unit " << event.unitId << " destroyed!\n";
            currentUnits--;
            auto it = std::remove_if(units.begin(), units.end(), 
                [event](const std::shared_ptr<Unit>& u) { return u->getId() == event.unitId; });
            units.erase(it, units.end());
        }
    }
    
    Unit* getUnitById(int id) {
        for (auto& u : units) if (u->getId() == id) return u.get();
        return nullptr;
    }
    Unit* getUnit(int index) { 
        if (index >= 0 && index < (int)units.size()) return units[index].get(); 
        return nullptr;
    }
};

// Перегрузка вывода (п.4 Этап 4)
inline std::ostream& operator<<(std::ostream& os, const Base& base) {
    os << "Base at (" << base.getX() << "," << base.getY() << ")";
    return os;
}