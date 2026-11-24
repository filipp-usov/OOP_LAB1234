#include "Command.h"

CreateCommand::CreateCommand(GameMediator* m, const std::string& t, int x, int y) 
    : mediator(m), type(t), x(x), y(y) {}
void CreateCommand::execute() { mediator->createUnit(type, x, y); }

MoveCommand::MoveCommand(GameMediator* m, int id, int dx, int dy) 
    : mediator(m), id(id), dx(dx), dy(dy) {}
void MoveCommand::execute() { 
    Unit* u = mediator->getBase()->getUnitById(id);
    if (u) mediator->moveUnit(id, u->getX() + dx, u->getY() + dy); 
}

AttackCmd::AttackCmd(GameMediator* m, int a, int t) 
    : mediator(m), attId(a), tgtId(t) {}
void AttackCmd::execute() { mediator->attack(attId, tgtId); }

BuildCmd::BuildCmd(GameMediator* m, const std::string& t, int x, int y) 
    : mediator(m), type(t), x(x), y(y) {}
void BuildCmd::execute() { mediator->buildBuilding(type, x, y); }

ProduceCmd::ProduceCmd(GameMediator* m, const std::string& t, int bid) 
    : mediator(m), type(t), id(bid) {}
void ProduceCmd::execute() { mediator->produceUnit(type, id); }

CollectCmd::CollectCmd(GameMediator* m, int id) 
    : mediator(m), id(id) {}
void CollectCmd::execute() { mediator->collectResources(id); }

HealCmd::HealCmd(GameMediator* m, int id) 
    : mediator(m), id(id) {}
void HealCmd::execute() { mediator->healUnit(id); }

StatsCmd::StatsCmd(GameMediator* m) 
    : mediator(m) {}
void StatsCmd::execute() { mediator->printStats(); }

DistributeCmd::DistributeCmd(GameMediator* m, const std::string& r, int a) 
    : mediator(m), res(r), amt(a) {}
void DistributeCmd::execute() { mediator->distributeResources(res, amt); }