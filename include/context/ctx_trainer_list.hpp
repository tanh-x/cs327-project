#ifndef CTX_TRAINER_LIST_H
#define CTX_TRAINER_LIST_H

#include <ncurses.h>
#include "core/game.hpp"
#include "context/ctx_main.hpp"

#define TRAINER_LIST_WINDOW_WIDTH 48
#define TRAINER_LIST_WINDOW_HEIGHT 10

class TrainerListContext : public AbstractContext {
public:
    TrainerListContext(MainContext* parent, std::vector<CorporealEntity*>* entityList);

    void start() override;

private:
    std::vector<CorporealEntity*>* entityList;

    void trainerListEntry();
};


bool trainerListInputHandler(int key);

#endif
