#include "world.hpp"
#include "enums/colors.hpp"
#include "util_ncurses.hpp"

void World::init(GameEngine* game) {
  ge = game;
}

Virgate& World::getVirgate(Triple& coord) {
  return provinces[coord.z].virgates[coord.y][coord.x];
}

void World::draw() {
  attron(COLOR_PAIR(WHITE_TEXT));
  attron(A_BOLD);
  current = 0;
  mvprintw(3, 2, provinces[current].eff.name.c_str());
  attroff(COLOR_PAIR(WHITE_TEXT));
  attroff(A_BOLD);    
  drawBox(4, 2, 27, 55, BORDER_TEXT);
  provinces[current].draw();
}
