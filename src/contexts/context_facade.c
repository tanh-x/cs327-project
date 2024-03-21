#include "contexts/context_facade.h"
#include "entity/event.h"
#include "contexts/ctx_world.h"
#include "contexts/ctx_trainer_list.h"


bool (* dispatchContextInputHandler(ContextType type))(int key) {
    switch (type) {
        case WORLD_CONTEXT: return worldContextInputHandler;
        case TRAINER_LIST_CONTEXT: return trainerListInputHandler;
        default: return NULL;
    }
}
