#include "GameField.h"
#include "Base.h" 
#include <iostream>
#include <algorithm>
#include <Windows.h>

GameField::GameField(int w, int h, int maxU) : width(w), height(h), maxUnits(maxU) { 
    initTerrain(w, h); 
}

void GameField::clear() { units.clear(); }

bool GameField::addUnit(Unit* unit) {
    if (!unit) return false;
    if (units.size() >= maxUnits) return false;
    if (unit->getX() < 0 || unit->getX() >= width || unit->getY() < 0 || unit->getY() >= height) return false;
    for (const auto& u : units) {
        if (u->getX() == unit->getX() && u->getY() == unit->getY()) return false;
    }
    units.push_back(std::shared_ptr<Unit>(unit)); 
    return true;
}

void GameField::removeUnit(int index) {
    if (index >= 0 && index < (int)units.size()) units.erase(units.begin() + index);
}

Unit* GameField::getUnit(int index) const {
    if (index >= 0 && index < (int)units.size()) return units[index].get();
    return nullptr;
}

void GameField::populateField(UnitFactory& factory, const std::string& type, int id, int x, int y) {
    Unit* newUnit = factory.createUnit(type, id, x, y);
    if (newUnit) addUnit(newUnit);
}

void GameField::printField() const {
    std::vector<std::vector<char>> grid(height, std::vector<char>(width, '.'));
    for (const auto& u : units) {
        char symbol = '?';
        std::string type = u->getType();
        if (type == "Swordsman") symbol = 'S';         
        else if (type == "Spearman") symbol = 'K';     
        else if (type == "LightArcher") symbol = 'L';  
        else if (type == "HeavyArcher") symbol = 'T';  
        else if (type == "Cataphract") symbol = 'C';   
        else if (type == "HorseArcher") symbol = 'H';  
        grid[u->getY()][u->getX()] = symbol; 
    }
    std::cout << "  "; for (int i = 0; i < width; ++i) std::cout << "X "; std::cout << "\n";
    for (int i = 0; i < height; ++i) {
        std::cout << "X ";
        for (int j = 0; j < width; ++j) std::cout << grid[i][j] << " ";
        std::cout << "X\n";
    }
    std::cout << "  "; for (int i = 0; i < width; ++i) std::cout << "X "; std::cout << "\n";
}

void GameField::initTerrain(int w, int h) {
    width = w; height = h;
    terrainMap.clear();
    for (int i = 0; i < h; ++i) {
        std::vector<std::shared_ptr<Terrain>> row;
        for (int j = 0; j < w; ++j) row.push_back(std::make_shared<Plains>());
        terrainMap.push_back(row);
    }
}

void GameField::setTerrain(int x, int y, std::shared_ptr<Terrain> t) {
    if (y >= 0 && y < height && x >= 0 && x < width) terrainMap[y][x] = t;
}

TerrainProxy GameField::getTerrainProxy(int x, int y) {
    if (x < 0 || x >= width || y < 0 || y >= height) return TerrainProxy(std::make_shared<Plains>());
    return TerrainProxy(terrainMap[y][x]);
}

void GameField::addObject(int x, int y, std::shared_ptr<NeutralObject> obj) {
    objects[{x, y}] = obj;
}

NeutralObject* GameField::getObjectAt(int x, int y) const {
    auto it = objects.find({x, y});
    return (it != objects.end()) ? it->second.get() : nullptr;
}

void GameField::printMap() const {
    printMapWithBase(-1, -1); 
}

void GameField::printMapWithBase(int baseX, int baseY, Base* basePtr) const {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    std::cout << "   ";
    for (int i = 0; i < width; ++i) std::cout << i << " ";
    std::cout << "\n  ";
    for (int i = 0; i < width; ++i) std::cout << "X ";
    std::cout << "\n";

    for (int i = 0; i < height; ++i) {
        std::cout << i << " X "; 
        for (int j = 0; j < width; ++j) {
            char symbol = '.'; 
            int color = 8;     

            
            if (terrainMap[i][j]) {
                std::string tName = terrainMap[i][j]->getName();
                if (tName == "Forest") { symbol = '*'; color = 10; }
                else if (tName == "Mountains") { symbol = '^'; color = 6; }
                else if (tName == "Swamp") { symbol = '~'; color = 1; }
            }

            
            NeutralObject* obj = getObjectAt(j, i);
            if (obj) {
                if (dynamic_cast<TreasureChest*>(obj)) { symbol = '$'; color = 14; }
                else if (dynamic_cast<Trap*>(obj)) { symbol = '!'; color = 12; }
                else if (dynamic_cast<Altar*>(obj)) { symbol = 'A'; color = 13; }
                else if (dynamic_cast<EnergySource*>(obj)) { symbol = 'E'; color = 11; }
            }

            
            for (const auto& u : units) {
                if (u->getX() == j && u->getY() == i) {
                    if (u->getType() == "Swordsman") symbol = 'S';
                    else if (u->getType() == "Spearman") symbol = 'K';
                    else if (u->getType() == "LightArcher") symbol = 'L';
                    else if (u->getType() == "HeavyArcher") symbol = 'T';
                    else if (u->getType() == "Cataphract") symbol = 'C';
                    else if (u->getType() == "HorseArcher") symbol = 'H';
                    color = 15; 
                }
            }

            
            if (basePtr) {
                for (auto& b : basePtr->getBuildings()) {
                    if (b->x == j && b->y == i) {
                        if (b->type == "Barracks") {
                            symbol = 'b'; 
                            color = 12; 
                        } else {
                            symbol = '#'; 
                            color = 12;
                        }
                    }
                }
            }

            
            if (j == baseX && i == baseY) { 
                symbol = 'B'; 
                color = 13; 
            }

            SetConsoleTextAttribute(hConsole, color);
            std::cout << symbol << " ";
        }
        SetConsoleTextAttribute(hConsole, 7);
        std::cout << "X\n"; 
    }
    std::cout << "  ";
    for (int i = 0; i < width; ++i) std::cout << "X ";
    std::cout << "\n";
}