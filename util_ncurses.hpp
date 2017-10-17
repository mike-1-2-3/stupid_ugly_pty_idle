
#ifndef SIM_GAME_NCURSES_UTIL
#define SIM_GAME_NCURSES_UTIL

#include "common.hpp"
#include "enums/colors.hpp"

void mclear(uint start_x, uint start_y, uint end_x, uint end_y);
void clearCenter();
void drawBox(uint y1, uint x1, uint y2, uint x2, color_pair color);

#endif
