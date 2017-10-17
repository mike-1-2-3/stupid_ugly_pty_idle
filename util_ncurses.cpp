
#include "util_ncurses.hpp"
#include <ncurses.h>

void mclear(uint start_x, uint start_y, uint end_x, uint end_y) {
  for (uint i = start_x; i<end_x; i++)
    for (uint j = start_y; j< end_y; j++)
      mvaddch(j, i, ' ');
}

void clearCenter() {
  for (uint i = 1; i < COLS - 1; i++)
    for (uint j = 3; j < LINES - 3; j++)
      mvaddch(j, i, ' ');  
}

void drawBox(uint y1, uint x1, uint y2, uint x2, color_pair color) {
  attron(COLOR_PAIR(color));
  // Lines
  move(y1,x1);   hline(ACS_HLINE, x2 - x1);
  move(y2,x1);   hline(ACS_HLINE, x2 - x1);
  move(y1,x1);   vline(ACS_VLINE, y2 - y1);
  move(y1,x2);   vline(ACS_VLINE, y2 - y1);

  // Corners
  move(y1,x1);  addch(ACS_ULCORNER);
  move(y2,x1);  addch(ACS_LLCORNER);
  move(y1,x2);  addch(ACS_URCORNER);
  move(y2,x2);  addch(ACS_LRCORNER);
  attroff(COLOR_PAIR(color));
}
