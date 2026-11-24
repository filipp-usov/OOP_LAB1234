#pragma once
#include <iostream>
#include <string>
#include "Base.h"
#include "GameField.h"
#include "Systems.h"
#include "LogSystem.h"

class GameMediator {
private:
    Base* base;
    GameField* field;
    CombatSystem combatSystem;
    ResourceSystem resourceSystem; 

public:
    GameMediator(Base* b, GameField* f) : base(b), field(f) {}

    bool moveUnit(int unitId, int newX, int newY) {
        Unit* unit = base->getUnitById(unitId);
        if (!unit) { std::cout << "[Mediator] Unit not found!\n"; return false; }

        TerrainProxy proxy = field->getTerrainProxy(newX, newY);
        if (proxy.isPassable(*unit)) {
            unit->move(newX, newY);
            std::cout << "[Mediator] Unit " << unitId << " moved to (" << newX << "," << newY << ")\n";
            Logger().log(LogEvent("INFO", "UNIT", "unit " + unit->getType() + " (ID=" + std::to_string(unitId) + ") moved to (" + std::to_string(newX) + "," + std::to_string(newY) + ")"));
            return true;
        } else {
            std::cout << "[Mediator] Path blocked!\n";
            return false;
        }
    }

    bool attack(int attackerId, int targetId) {
        Unit* att = base->getUnitById(attackerId);
        Unit* tgt = base->getUnitById(targetId);
        if (!att || !tgt) { std::cout << "[Mediator] Invalid units!\n"; return false; }
        combatSystem.resolveCombat(att, tgt);
        return true;
    }

    void createUnit(const std::string& type, int x, int y) { base->createUnit(type, x, y); }
    void collectResources(int unitId) { base->gatherResources(); }
    void healUnit(int unitId) { 
        Unit* u = base->getUnitById(unitId);
        if (u) u->getHealth().increase(30);
    }
    void buildBuilding(const std::string& type, int x, int y) { base->buildBuilding(type, x, y); }
    void produceUnit(const std::string& type, int id) { base->produceUnit(type, id); }
    void distributeResources(const std::string& res, int amt) { base->distributeResources(res, amt); }
    void printStats() { base->printStats(); resourceSystem.printStats(); }
    Base* getBase() { return base; }
};