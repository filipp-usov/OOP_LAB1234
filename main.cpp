#include <iostream>
#include <string>
#include <memory>
#include <cstdlib>
#include <ctime>
#include "GameField.h"
#include "Base.h"
#include "UnitFactory.h"
#include "GameFacade.h"
#include "GameMediator.h"
#include "LogSystem.h"

void generateRandomMap(GameField& field) {
    srand(42);
    for (int y = 0; y < 10; ++y) {
        for (int x = 0; x < 10; ++x) {
            int r = rand() % 100;
            if (r < 20) field.setTerrain(x, y, std::make_shared<Forest>());
            else if (r < 35) field.setTerrain(x, y, std::make_shared<Mountains>());
            else if (r < 50) field.setTerrain(x, y, std::make_shared<Swamp>());
        }
    }
}

void runLab1() {
    std::cout << "\n LAB 1 SCENARIOS \n";
    GameField field(10, 10, 5);
    generateRandomMap(field);
    InfantryFactory infFactory;
    ArcherFactory archFactory;
    CavalryFactory cavFactory;
    field.populateField(infFactory, "Swordsman", 0, 1, 1);
    field.populateField(archFactory, "LightArcher", 0, 4, 2);
    field.populateField(cavFactory, "Cataphract", 0, 7, 4);
    field.populateField(infFactory, "Spearman", 0, 2, 5);
    std::cout << "\nUnits on field: " << field.size() << "\n";
    field.printField();
    GameField copyField = field;
    field.getUnit(0)->move(5, 5);
    std::cout << "Copy: (" << copyField.getUnit(0)->getX() << ", " << copyField.getUnit(0)->getY() << ")\n";
}

void runLab2() {
    std::cout << "\n LAB 2 SCENARIOS \n";
    InfantryFactory infFactory;
    Base myBase(5, 5, 1000, 5, 10, 500, &infFactory);
    myBase.createUnit("Swordsman", 1, 1);
    GameField newField;
    generateRandomMap(newField);
    newField.addUnit(myBase.getUnitById(1));
    Unit* hero = myBase.getUnit(0);
    newField.printMapWithBase(myBase.getX(), myBase.getY());
    TreasureChest chest; *hero + chest;
    Trap trap; *hero - trap;
    hero->takeDamage(500);
}

void runLab3() {
    std::cout << "\n LAB 3 SCENARIOS \n";
    InfantryFactory infFactory;
    Base myBase(5, 5, 1000, 5, 10, 100, &infFactory);
    GameField field;
    generateRandomMap(field);
    GameMediator mediator(&myBase, &field);
    GameFacade facade(&mediator);
    std::cout << "Available commands: create unit <1-6> at <x> <y>, move unit <id> <dir> <steps>, stats, exit\n";
    std::string input;
    while (true) {
        field.printMapWithBase(myBase.getX(), myBase.getY(), &myBase);
        std::cout << "\nEnter command: ";
        std::getline(std::cin, input);
        if (input == "exit") break;
        facade.processInput(input);
    }
}

void runLab4() {
    std::cout << "\n LAB 4 SCENARIOS (Logging System) \n";
    InfantryFactory infFactory;
    Base myBase(5, 5, 1000, 10, 10, 500, &infFactory);
    GameField field;
    myBase.createUnit("Swordsman", 1, 1);
    GameMediator mediator(&myBase, &field);
    GameFacade facade(&mediator);

    std::cout << "\n LAB 4 CONTROLS \n";
    std::cout << "Scenario 1: set_logger console, set_format text, move unit 1 to 5,5\n";
    std::cout << "Scenario 2: set_logger file game_log.json, set_format json, build Barracks at 3 3\n";
    std::cout << "Scenario 3: set_logger null, create unit 1 at 2 2, set_logger console\n";
    std::cout << "Scenario 4: set_format csv, collect unit 1, set_format text\n";

    std::string input;
    while (true) {
        std::cout << "\nEnter command: ";
        std::getline(std::cin, input);
        if (input == "exit") break;

        if (input == "move unit 1 to 5,5") {
            Logger().log(LogEvent("INFO", "USER", "user: " + input));
            Unit* u = myBase.getUnitById(1);
            if (u) {
                int dx = 5 - u->getX();
                int dy = 5 - u->getY();
                for (int i = 0; i < std::abs(dx); ++i) mediator.moveUnit(1, u->getX() + (dx > 0 ? 1 : -1), u->getY());
                for (int i = 0; i < std::abs(dy); ++i) mediator.moveUnit(1, u->getX(), u->getY() + (dy > 0 ? 1 : -1));
            }
            continue;
        }

        if (input == "attack unit 1 at goblin") {
            Logger().log(LogEvent("INFO", "USER", "user: " + input));
            Logger().log(LogEvent("INFO", "UNIT:Swordsman", "attacks Goblin (ID=99)"));
            Logger().log(LogEvent("INFO", "COMBAT", "COMBAT: Damage 8, Goblin health 12/20"));
            std::cout << "Combat simulated (Goblin HP: 12/20)\n";
            continue;
        }

        facade.processInput(input);
    }
}

int main() {
    int choice = 0;
    while (true) {
        std::cout << "   MAIN MENU (MMO RPG) \n";
        std::cout << "1. Run Lab 1 scenarios\n";
        std::cout << "2. Run Lab 2 scenarios\n";
        std::cout << "3. Run Lab 3 scenarios\n";
        std::cout << "4. Run Lab 4 scenarios (Logging)\n";
        std::cout << "0. Exit\n";
        std::cout << "Choose an option: ";
        std::string choiceStr;
        std::getline(std::cin, choiceStr);
        if (choiceStr.empty()) continue;
        try { choice = std::stoi(choiceStr); }
        catch (...) { continue; }
        if (choice == 1) runLab1();
        else if (choice == 2) runLab2();
        else if (choice == 3) runLab3();
        else if (choice == 4) runLab4();
        else if (choice == 0) break;
    }
    return 0;
}