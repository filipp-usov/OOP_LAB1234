#pragma once
#include <string>
#include <iostream>

class Building {
public:
    int id; 
    std::string type;
    int health;
    int maxHealth;
    bool isUnderConstruction;
    int buildProgress;
    int x, y;

    
    Building(int id, std::string t, int hp, int bx, int by) 
        : id(id), type(t), health(hp), maxHealth(hp), isUnderConstruction(true), buildProgress(0), x(bx), y(by) {}

    void progressBuild() {
        if (isUnderConstruction) {
            buildProgress += 34;
            if (buildProgress >= 100) {
                buildProgress = 100;
                isUnderConstruction = false;
                std::cout << "[Building] " << type << " (ID: " << id << ") construction completed!\n";
            } else {
                std::cout << "[Building] " << type << " construction progress: " << buildProgress << "%\n";
            }
        }
    }
};