
#ifndef SIM_GAME_CREATURE
#define SIM_GAME_CREATURE

#include "common.hpp"
#include "combatant.hpp"
#include "enums/CreatureIndex.hpp"

struct Creature {
  Effable eff;
  Combatant combat;
  uint elusiveness;
  uint spawn_min;
  uint spawn_max;
};

#endif
