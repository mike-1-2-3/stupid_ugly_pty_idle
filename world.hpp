
#ifndef SIM_GAME_WORLD
#define SIM_GAME_WORLD

#include "province.hpp"
class GameEngine;

class World {
public :
  vector<Province> provinces;
  uint current;
  GameEngine* ge;
  Virgate& getVirgate(Triple& coord);
  void init(GameEngine* game);
  void up(){provinces[current].up();};
  void down(){provinces[current].down();};
  void left(){provinces[current].left();};
  void right(){provinces[current].right();};
  void draw();
};
#endif
