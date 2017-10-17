
#include "menu.hpp"


void Menu::selectAll(vector<string>& menu_items) {
  for (uint i = 0; i < menu_items.size(); i++ )
    items.push_back(&menu_items[i]);
  init();
}

bool Menu::up() {
  if (selected > first) {
    selected--;
    return true;
  }
  else 
    if (selected == first) {
      if (first > 0) {
	first--;
	selected--;
	last_visible--;
	return true;
      }
    }    
  return false;
}

bool Menu::down() {
  if (selected < last_visible) {
    selected++;
    return true;
  }
  else 
    if (selected == last_visible) {
      if (last_visible + 1 < items.size() ) {
	first++;
	selected++;
	last_visible++;
	return true;
      }
    }    
  return false;

}

void Menu::init() {
  selected = 0;
  first = 0;
}
