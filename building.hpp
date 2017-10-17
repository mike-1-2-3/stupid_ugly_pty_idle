
#ifndef SIM_GAME_BUILDING
#define SIM_GAME_BUILDING

class Building {
  Effable eff;
  CountableIndex id;
  uint time; // days
  uint labor; // labor per day
  vector<CountableIndexQuantity> consumed_materials;
  vector<CountableIndexQuantity> prereqs;
}

#endif
