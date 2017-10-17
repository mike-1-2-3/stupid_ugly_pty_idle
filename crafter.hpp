
#ifndef SIM_GAME_CRAFTER
#define SIM_GAME_CRAFTER

struct Recipe {
  vector<Stack> inputs;
  vector<Stack> outputs;
}

struct Crafter {
  uint funds;
  vector<Recipe> crafts;
  uint current_craft;
};

#endif
