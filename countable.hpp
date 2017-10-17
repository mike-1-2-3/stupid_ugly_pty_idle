
#ifndef SIM_GAME_COUNTABLE
#define SIM_GAME_COUNTABLE

#include "common.hpp"
#include "enums/CountableIndex.hpp"
#include "enums/CountableCategory.hpp"

struct Countable {
  Effable eff;
  vector<uint> properties;
};

uint isFood(uint t);

#endif
