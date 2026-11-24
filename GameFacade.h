#pragma once
#include <memory>
#include <iostream>
#include "GameMediator.h"
#include "Handlers.h"
#include "CommandHistory.h"
#include "LogSystem.h"

class GameFacade {
private:
    GameMediator* mediator;
    std::shared_ptr<CommandHandler> chainStart;
    CommandHistory history;

    void setupChain() {
        auto createH = std::make_shared<CreateHandler>();
        auto moveH = std::make_shared<MoveHandler>();
        auto attackH = std::make_shared<AttackHandler>();
        auto buildH = std::make_shared<BuildHandler>();
        auto produceH = std::make_shared<ProduceHandler>();
        auto collectH = std::make_shared<CollectHandler>();
        auto healH = std::make_shared<HealHandler>();
        auto distH = std::make_shared<DistributeHandler>();
        auto statsH = std::make_shared<StatsHandler>();
        auto logH = std::make_shared<LogHandler>();

        createH->setNext(moveH);
        moveH->setNext(attackH);
        attackH->setNext(buildH);
        buildH->setNext(produceH);
        produceH->setNext(collectH);
        collectH->setNext(healH);
        healH->setNext(distH);
        distH->setNext(statsH);
        statsH->setNext(logH);
        chainStart = createH;
    }

public:
    GameFacade(GameMediator* m) : mediator(m) { setupChain(); }

    void processInput(const std::string& input) {
        Logger().log(LogEvent("INFO", "USER", "user: " + input));

        if (input == "end_turn") {
            mediator->getBase()->tick();
            return;
        }
        if (input == "undo") {
            history.undo();
            return;
        }
        if (!chainStart->handle(input, mediator, history)) {
            std::cout << "Unknown command.\n";
        }
    }
};