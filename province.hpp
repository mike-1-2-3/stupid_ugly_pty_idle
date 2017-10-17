
#ifndef SIM_GAME_PROVINCE
#define SIM_GAME_PROVINCE

#include <ncurses.h>
#include "common.hpp"
#include "virgate.hpp"
#include "food.hpp"
#include "countable.hpp"

class GameEngine;

class Province {
public:
  void init(uint x, uint y);
  void draw();
  void up();
  void down();
  void left();
  void right();
  void generateBailedoire();
  Effable eff;
  static GameEngine* ge;
  vector<vector<Virgate>> virgates;

  vector<uint> monster_populations;
  vector<uint> sentient_populations;
  vector<uint> supply;
  vector<uint> demand;
  bool no_free_land;

  void genRandomForest(Virgate& v);
  void genRandomPlain(Virgate& v);
  void genRandomMountain(Virgate& v);
  void genRandomRiver(Virgate& v);
  void genRandomBog(Virgate& v);

  // Display
  chtype** map;
  uint width, height, tlx, tly, vtlx, vtly, vwidth, vheight;
  uint cx, cy;
  uint capital_x, capital_y;
  ~Province();
};

#endif
