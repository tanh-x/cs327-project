#ifndef CTX_BUILDING_H
#define CTX_BUILDING_H

#include "core/game_manager.hpp"
#include "ctx_main.hpp"

class BuildingContext : public AbstractContext {
public:
    BuildingContext(MainContext* parent, TileType type);

    void start() override;
};

#endif
