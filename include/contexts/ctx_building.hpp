#ifndef CTX_BUILDING_H
#define CTX_BUILDING_H

#include "core/game_manager.hpp"
#include "ctx_world.hpp"

class BuildingContext : public AbstractContext {
public:
    BuildingContext(WorldContext* parent, TileType type);

    void start() override;
};

#endif
