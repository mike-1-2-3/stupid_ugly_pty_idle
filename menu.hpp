#ifndef SIM_GAME_MENU
#define SIM_GAME_MENU

#include "common.hpp"

struct Menu {
  void selectAll(vector<string>& menu_items);
  template<typename T> 
  void selectAll(T& menu_items) {
    for (uint i = 0; i < menu_items.size(); i++ ) 
      items.push_back(&menu_items[i].eff.name);
    init();
  };
  bool up();
  bool down();
  void init();
  bool focus;
  vector<string*> items;
  uint first;
  uint selected;
  uint last_visible;
  int start_y;
  int end_y;
  int start_x;
  int end_x;
};

#endif
