#ifndef CTX_TRAINER_LIST_H
#define CTX_TRAINER_LIST_H

#include <ncurses.h>
#include "core/game.hpp"
#include "contexts/ctx_world.hpp"

#define TRAINER_LIST_WINDOW_WIDTH 48
#define TRAINER_LIST_WINDOW_HEIGHT 10

class TrainerListContext : public AbstractContext {
public:
    std::vector<AbstractEntity*>* entityList;

    TrainerListContext(WorldContext* parent, std::vector<AbstractEntity*>* entityList);

    void start() override;

private:
    void trainerListEntry();
};


bool trainerListInputHandler(int key);

#endif
