#ifndef CONTEXT_TYPE_H
#define CONTEXT_TYPE_H
// See context.h

typedef enum __attribute__ ((__packed__)) {
    MAIN_MENU_CONTEXT,
    WORLD_CONTEXT,
    BATTLE_CONTEXT,
    TRAINER_LIST_CONTEXT
} ContextType;

#endif
