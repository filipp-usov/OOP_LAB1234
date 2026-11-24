#pragma once
#include "Unit.h"
#include <iostream>
#include "LogSystem.h"

class CombatSystem {
public:
    bool resolveCombat(Unit* attacker, Unit* target) {
        if (!attacker || !target) return false;

        attacker->attackTarget(*target); 

        int rawDmg = attacker->getAttack().getValue();
        std::cout << "[CombatSystem] " << attacker->getType() << " hits " << target->getType() << " for " << rawDmg << " dmg!\n";

        Logger().log(LogEvent("INFO", "COMBAT", "COMBAT: unit " + std::to_string(attacker->getId()) + " deals " + std::to_string(rawDmg) + " base damage to unit " + std::to_string(target->getId())));

        target->takeDamage(rawDmg);

        if (target->getHealth().isZero()) {
            Logger().log(LogEvent("ALERT", "COMBAT", "COMBAT: Unit " + std::to_string(target->getId()) + " destroyed!"));
            return true;
        }
        return false;
    }
};

class ResourceSystem {
public:
    int wood = 100, metal = 100, energy = 100, gold = 100, leather = 100;
    void addResources(int w, int m, int e, int g, int l) { wood += w; metal += m; energy += e; gold += g; leather += l; }
    void printStats() { std::cout << "Resources: Wood " << wood << " | Metal " << metal << " | Energy " << energy << " | Gold " << gold << " | Leather " << leather << "\n"; }
};