
#ifndef SIM_GAME_PLAYER
#define SIM_GAME_PLAYER

#include "common.hpp"
#include "person.hpp"
#include "world.hpp"

class GameEngine;

struct Player {
  void init(uint num_positions, World* w, GameEngine* game);
  void assignRetinue(uint position, uint id);
  void addVirgate(Triple coord);
  vector<uint> retinue;
  vector<bool> vacant;
  vector<uint> controlled_provinces;
  uint current_province;
  World* world;
  GameEngine * ge;
  Triple getRandomVirgate();
  Triple nextFreeVirgate();
};

#endif
