#pragma once
#include <string>
#include <iostream>
#include "GameMediator.h"

class Command {
public:
    virtual ~Command() = default;
    virtual void execute() = 0;
    virtual void undo() {} 
};

class CreateCommand : public Command {
    GameMediator* mediator; std::string type; int x, y;
public:
    CreateCommand(GameMediator* m, const std::string& t, int x, int y);
    void execute() override;
};

class MoveCommand : public Command {
    GameMediator* mediator; int id; int dx, dy;
public:
    MoveCommand(GameMediator* m, int id, int dx, int dy);
    void execute() override;
};

class AttackCmd : public Command {
    GameMediator* mediator; int attId, tgtId;
public:
    AttackCmd(GameMediator* m, int a, int t);
    void execute() override;
};

class BuildCmd : public Command {
    GameMediator* mediator; std::string type; int x, y;
public:
    BuildCmd(GameMediator* m, const std::string& t, int x, int y);
    void execute() override;
};

class ProduceCmd : public Command {
    GameMediator* mediator; std::string type; int id;
public:
    ProduceCmd(GameMediator* m, const std::string& t, int bid);
    void execute() override;
};

class CollectCmd : public Command {
    GameMediator* mediator; int id;
public:
    CollectCmd(GameMediator* m, int id);
    void execute() override;
};

class HealCmd : public Command {
    GameMediator* mediator; int id;
public:
    HealCmd(GameMediator* m, int id);
    void execute() override;
};

class StatsCmd : public Command {
    GameMediator* mediator;
public:
    StatsCmd(GameMediator* m);
    void execute() override;
};

class DistributeCmd : public Command {
    GameMediator* mediator; std::string res; int amt;
public:
    DistributeCmd(GameMediator* m, const std::string& r, int a);
    void execute() override;
};