#pragma once
#include <vector>
#include <string>
#include <map>
#include <memory>
#include "Unit.h"
#include "UnitFactory.h"
#include "Terrain.h"
#include "NeutralObject.h"

class Base; 

class GameField {
private:
    int width;
    int height;
    int maxUnits;
    std::vector<std::shared_ptr<Unit>> units; 
    std::vector<std::vector<std::shared_ptr<Terrain>>> terrainMap;
    std::map<std::pair<int, int>, std::shared_ptr<NeutralObject>> objects;

public:
    GameField(int w = 10, int h = 10, int maxU = 10);
    ~GameField() = default;

    bool addUnit(Unit* unit);
    void removeUnit(int index);
    Unit* getUnit(int index) const;
    int size() const { return (int)units.size(); }
    bool empty() const { return units.empty(); }
    void clear();
    void populateField(UnitFactory& factory, const std::string& type, int id, int x, int y);
    void printField() const;

    class GameFieldIterator {
    private:
        const GameField* field;
        int currentIndex;
    public:
        GameFieldIterator(const GameField* f, int index) : field(f), currentIndex(index) {}
        bool operator!=(const GameFieldIterator& other) const { return currentIndex != other.currentIndex; }
        Unit* operator*() const { return field->units[currentIndex].get(); }
        GameFieldIterator& operator++() { currentIndex++; return *this; }
    };
    GameFieldIterator begin() const { return GameFieldIterator(this, 0); }
    GameFieldIterator end() const { return GameFieldIterator(this, (int)units.size()); }

    void initTerrain(int w, int h);
    void setTerrain(int x, int y, std::shared_ptr<Terrain> t);
    TerrainProxy getTerrainProxy(int x, int y);
    void addObject(int x, int y, std::shared_ptr<NeutralObject> obj);
    NeutralObject* getObjectAt(int x, int y) const;
    
    void printMap() const; 
    void printMapWithBase(int baseX, int baseY, Base* basePtr = nullptr) const; 
};