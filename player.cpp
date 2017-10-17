#include "player.hpp"
#include "game_engine.hpp"
#include "world.hpp"

void Player::assignRetinue(uint position, uint id) {
  retinue[position] = id;
  vacant[position] = false;
}

void Player::init(uint num_positions, World* w, GameEngine* game) {
  world = w;
  ge = game;
  retinue.resize(num_positions);
  vacant.assign(num_positions, true);
  current_province = 0;
}

Triple Player::getRandomVirgate() {
  Triple ret;
  uint p = 0;
  current_province = 0;
  Province& province = world->provinces[p];
  ret.z = 0;
  bool found = false;
  uint& x = ret.x;
  uint& y = ret.y;

  for (uint i = 0; i < 5 && found == false; i++) {
    x = rand() % province.width;
    y = rand() % province.height;
    cerr << " x, y: " << x <<"," << y<<"\n"<<std::flush;
    if (!province.virgates[y][x].in_use)
      found = true;
  }
  // todo - pick most lucrative out of five
  cerr << "virg picked: "<<y <<x;
  if (found)
    return ret;
  
  else return nextFreeVirgate();
}

Triple Player::nextFreeVirgate() {
  Triple ret;
  ret.z = 0;
  uint i = 0;
  uint& x = ret.x;
  uint& y = ret.y;
  for (auto& prov : world->provinces) {
    current_province = i;
    if (prov.no_free_land)
      continue;
    for (y = prov.capital_y; y < prov.height; y++)
      for (x = 0; x < prov.width; x++)
	if (!prov.virgates[y][x].in_use)
	  return ret;
    for (y = prov.capital_y; y > 0; y--)
      for (x = 0; x < prov.width; x++)
	if (!prov.virgates[y][x].in_use)
	  return ret;
    prov.no_free_land = true;
    i++;
  }
  
  cerr << "dev hasnt programmed this part yet\n";
}

void Player::addVirgate(Triple coord) {
  Virgate& v = world->getVirgate(coord);
  v.in_use = true;
  for (auto& r : v.resources) {
    if (isFood(r.type) != 0) {
      uint food_idx = isFood(r.type);
      for (uint i = 0; i < ge->foods[food_idx].nutrients.size(); i++)
	world->provinces[current_province].supply[ge->firsts[CC_Nutrients] + i] += 
	  r.amount * ge->foods[food_idx].nutrients[i];
    }
    world->provinces[current_province].supply[r.type] += r.amount;
  }
}
