#pragma once
#include <vector>
#include <memory>
#include "Command.h"

class CommandHistory {
private:
    std::vector<std::shared_ptr<Command>> history;
public:
    void push(std::shared_ptr<Command> cmd) { history.push_back(cmd); }
    void undo() {
        if (history.empty()) { std::cout << "Nothing to undo.\n"; return; }
        history.back()->undo();
        history.pop_back();
        std::cout << "Last command undone.\n";
    }
    std::shared_ptr<Command> getLastCommand() {
        if (history.empty()) return nullptr;
        return history.back();
    }
};