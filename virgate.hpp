
#ifndef SIM_GAME_VIRGATE
#define SIM_GAME_VIRGATE

#include "common.hpp"
#include "enums/LandTypeIndex.hpp"

struct ResourcePair {
  uint type;
  uint amount;
};

struct Virgate {
  LandTypeIndex type;
  Effable eff;
  vector<ResourcePair> resources;
  bool in_use;
};


#endif
