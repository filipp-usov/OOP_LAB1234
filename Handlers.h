#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <memory>
#include "Command.h"
#include "CommandHistory.h"
#include "LogSystem.h"

class CommandHandler {
protected:
    std::shared_ptr<CommandHandler> nextHandler;
public:
    void setNext(std::shared_ptr<CommandHandler> handler) { nextHandler = handler; }
    virtual bool handle(const std::string& input, GameMediator* mediator, CommandHistory& history) = 0;
};

class LogHandler : public CommandHandler {
public:
    bool handle(const std::string& input, GameMediator* mediator, CommandHistory& history) override {
        std::istringstream iss(input);
        std::string cmd, arg;
        iss >> cmd >> arg;

        if (cmd == "set_logger") {
            Logger().setLoggerFromString(arg);
            std::cout << "[Log] Logger switched to " << arg << "\n";
            return true;
        }
        if (cmd == "set_format") {
            Logger().setFormatterFromString(arg);
            std::cout << "[Log] Format switched to " << arg << "\n";
            return true;
        }
        if (nextHandler) return nextHandler->handle(input, mediator, history);
        return false;
    }
};

class CreateHandler : public CommandHandler {
public:
    bool handle(const std::string& input, GameMediator* mediator, CommandHistory& history) override {
        std::istringstream iss(input); std::string cmd, unit, type, at; int x, y;
        if (iss >> cmd >> unit >> type >> at >> x >> y && cmd == "create" && unit == "unit" && at == "at") {
            if (type == "1") type = "Swordsman"; else if (type == "2") type = "Spearman";
            else if (type == "3") type = "LightArcher"; else if (type == "4") type = "HeavyArcher";
            else if (type == "5") type = "Cataphract"; else if (type == "6") type = "HorseArcher";
            auto cmdPtr = std::make_shared<CreateCommand>(mediator, type, x, y);
            cmdPtr->execute(); history.push(cmdPtr); return true;
        }
        if (nextHandler) return nextHandler->handle(input, mediator, history);
        return false;
    }
};

class MoveHandler : public CommandHandler {
public:
    bool handle(const std::string& input, GameMediator* mediator, CommandHistory& history) override {
        std::istringstream iss(input); std::string cmd, unit; int id, steps; char dir;
        if (iss >> cmd >> unit >> id >> dir >> steps && cmd == "move" && unit == "unit") {
            for (int i = 0; i < steps; ++i) {
                int dx = (dir == 'd' ? 1 : (dir == 'a' ? -1 : 0));
                int dy = (dir == 's' ? 1 : (dir == 'w' ? -1 : 0));
                auto cmdPtr = std::make_shared<MoveCommand>(mediator, id, dx, dy);
                cmdPtr->execute(); history.push(cmdPtr);
            }
            return true;
        }
        if (nextHandler) return nextHandler->handle(input, mediator, history);
        return false;
    }
};

class AttackHandler : public CommandHandler {
public:
    bool handle(const std::string& input, GameMediator* mediator, CommandHistory& history) override {
        std::istringstream iss(input); std::string cmd, unit; int a, t;
        if (iss >> cmd >> unit >> a >> t && cmd == "attack" && unit == "unit") {
            auto cmdPtr = std::make_shared<AttackCmd>(mediator, a, t);
            cmdPtr->execute(); history.push(cmdPtr); return true;
        }
        if (nextHandler) return nextHandler->handle(input, mediator, history);
        return false;
    }
};

class BuildHandler : public CommandHandler {
public:
    bool handle(const std::string& input, GameMediator* mediator, CommandHistory& history) override {
        std::istringstream iss(input); std::string cmd, type, at; int x, y;
        if (iss >> cmd >> type >> at >> x >> y && cmd == "build" && at == "at") {
            auto cmdPtr = std::make_shared<BuildCmd>(mediator, type, x, y);
            cmdPtr->execute(); history.push(cmdPtr); return true;
        }
        if (nextHandler) return nextHandler->handle(input, mediator, history);
        return false;
    }
};

class ProduceHandler : public CommandHandler {
public:
    bool handle(const std::string& input, GameMediator* mediator, CommandHistory& history) override {
        std::istringstream iss(input); std::string cmd, unit, type, in; int id;
        if (iss >> cmd >> unit >> type >> in >> id && cmd == "produce" && unit == "unit" && in == "in") {
            if (type == "1") type = "Swordsman"; else if (type == "2") type = "Spearman";
            else if (type == "3") type = "LightArcher"; else if (type == "4") type = "HeavyArcher";
            else if (type == "5") type = "Cataphract"; else if (type == "6") type = "HorseArcher";
            auto cmdPtr = std::make_shared<ProduceCmd>(mediator, type, id);
            cmdPtr->execute(); history.push(cmdPtr); return true;
        }
        if (nextHandler) return nextHandler->handle(input, mediator, history);
        return false;
    }
};

class CollectHandler : public CommandHandler {
public:
    bool handle(const std::string& input, GameMediator* mediator, CommandHistory& history) override {
        std::istringstream iss(input); std::string cmd, unit; int id;
        if (iss >> cmd >> unit >> id && cmd == "collect") {
            auto cmdPtr = std::make_shared<CollectCmd>(mediator, id);
            cmdPtr->execute(); history.push(cmdPtr); return true;
        }
        if (nextHandler) return nextHandler->handle(input, mediator, history);
        return false;
    }
};

class HealHandler : public CommandHandler {
public:
    bool handle(const std::string& input, GameMediator* mediator, CommandHistory& history) override {
        std::istringstream iss(input); std::string cmd, unit; int id;
        if (iss >> cmd >> unit >> id && cmd == "heal") {
            auto cmdPtr = std::make_shared<HealCmd>(mediator, id);
            cmdPtr->execute(); history.push(cmdPtr); return true;
        }
        if (nextHandler) return nextHandler->handle(input, mediator, history);
        return false;
    }
};

class StatsHandler : public CommandHandler {
public:
    bool handle(const std::string& input, GameMediator* mediator, CommandHistory& history) override {
        if (input == "stats") { StatsCmd(mediator).execute(); return true; }
        if (nextHandler) return nextHandler->handle(input, mediator, history);
        return false;
    }
};

class DistributeHandler : public CommandHandler {
public:
    bool handle(const std::string& input, GameMediator* mediator, CommandHistory& history) override {
        std::istringstream iss(input); std::string cmd, res; int amt;
        if (iss >> cmd >> res >> amt && cmd == "distribute") {
            auto cmdPtr = std::make_shared<DistributeCmd>(mediator, res, amt);
            cmdPtr->execute(); history.push(cmdPtr); return true;
        }
        if (nextHandler) return nextHandler->handle(input, mediator, history);
        return false;
    }
};