#ifndef CONTEXT_TYPE_H
#define CONTEXT_TYPE_H
// See context.h

enum class ContextType : uint8_t {
    MAIN_MENU_CONTEXT [[maybe_unused]],
    WORLD_CONTEXT,
    BATTLE_CONTEXT,
    BUILDING_CONTEXT,
    TRAINER_LIST_CONTEXT
};

#endif
