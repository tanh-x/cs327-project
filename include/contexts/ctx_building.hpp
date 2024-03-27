#ifndef CTX_BUILDING_H
#define CTX_BUILDING_H

#include "core/game_manager.hpp"
#include "ctx_world.hpp"

class BuildingContext : public AbstractContext {
    BuildingContext(WorldContext* parent, TileType type);

private:
    static Rect2D getDimensions();
};

void enterPlaceholderBuilding(TileType type);

#endif
