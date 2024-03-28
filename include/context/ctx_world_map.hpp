#ifndef CTX_WORLD_MAP_H
#define CTX_WORLD_MAP_H


#include "context/abstract_context.hpp"
#include "ctx_world.hpp"

class WorldMapContext : public AbstractContext {
public:
    WorldMapContext(AbstractContext* parent, World* world);

    void start() override;

private:
    World* world;

    void worldMapEntry();

    void drawWorldMap(int pivotX, int pivotY);
};


#endif
