#include <stdio.h>
#include "entity/npc/pacer.h"

PacerSoul* constructPacerSoul() {
    PacerSoul* soul = malloc(siezof(PacerSoul));
    return soul;
}