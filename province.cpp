#include "province.hpp"
#include "enums/colors.hpp"
#include "enums/CountableCategory.hpp"
#include <cstring>
#include <fstream>
#include "util_ncurses.hpp"
#include "util.hpp"
#include "game_engine.hpp"
/*
vector<Countable>* Province::stuff = NULL;
vector<Food>* Province::foods = NULL;
vector<string>* Province::land_names = NULL;
vector<uint>* Province::firsts = NULL;
vector<uint>* Province::lasts = NULL;
*/
GameEngine* Province::ge = nullptr;

Province::~Province() {
  for (uint i = 0; i < height; i++)
    free(map[i]);
  free(map);
}

void Province::init(uint x, uint y) {
  width = x;
  height = y;
  virgates.resize(height);
  for (auto& row : virgates)
    row.resize(width);
  vwidth = 50;
  vheight = 20;
  cx = 0;
  cy = 0;
  tlx = 0;
  tly = 0;
  vtlx = 4;
  vtly = 6;
  no_free_land = false;
  
  map = (chtype**)malloc(height * sizeof(chtype *));
  for (chtype i = 0; i < height; i++)
    map[i] = (chtype*)malloc(width * sizeof(chtype));
  supply.assign(ge->stuff.size(), 0);
  demand.assign(ge->stuff.size(), 0);
  sentient_populations.assign(ge->sentients.size(), 0);
  char c;
  uint i = 0;
  uint j = 0;
  std::fstream inf("map.txt", std::ios::in); 
  vector<string>& land = ge->land_names;
  while (inf.get(c) && i < width) {

    if (c == '\n')
      continue;

    Virgate& tile = virgates[j][i];
    tile.in_use = false;
    switch(c) {
      case ' ' :
	map[j][i] = c | COLOR_PAIR(GREEN_SOLID);
	tile.type = LTI_plains; 
	tile.eff.name = land[LTI_plains];
	genRandomPlain(tile);
	break;
      case 'B' :
	map[j][i] = c | COLOR_PAIR(GOLD_SOLID);
	tile.type = LTI_bog; 
	tile.eff.name = land[LTI_bog];
	genRandomBog(tile);
	break;
      case 'F' :
	map[j][i] = ' ' | COLOR_PAIR(FOREST_SOLID);
	tile.type = LTI_forest;
	tile.eff.name = land[LTI_forest];
	genRandomForest(tile);
	break;
      case 'R' :
	map[j][i] = c | COLOR_PAIR(BLUE_SOLID);
	tile.type = LTI_river;
	tile.eff.name = land[LTI_river];
	genRandomRiver(tile);
	break;
      case 'M' :
	map[j][i] = c | COLOR_PAIR(GREY_SOLID);
	tile.type = LTI_mountains;
	tile.eff.name = land[LTI_mountains];
	genRandomMountain(tile);
	break;
      case 'C' :
	map[j][i] = c | COLOR_PAIR(GOLD_SOLID);
	tile.type = LTI_city;
	tile.eff.name = land[LTI_city];
	tile.in_use = true;
	capital_x = j;
	capital_y = i;
	break;
      default :
	map[j][i] = c | COLOR_PAIR(BLOODY_TEXT);
	cerr << "Unrecognized map character\n";
	break;
    }

    i++;
    if (i >= width) {
      j++;
      i = 0;
    }
  }
}

void Province::up (){
  if (cy > 0)
    cy--;
  else if (tly > 0)
    tly--;
  draw();
}

void Province::down (){
  if (cy < vheight - 1)
    cy++;
  else if (tly < height - vheight)
    tly++;
  draw();
}

void Province::left (){
  if (cx > 0)
    cx--;
  else if (tlx > 0)
    tlx--;
  draw();
}

void Province::right (){
  if (cx < vwidth - 1)
    cx++;
  else if (tlx < width - vwidth)
    tlx++;
  draw();
}

void Province::draw() {
  attron(COLOR_PAIR(GREEN_TEXT));
  mvaddch(vtly-1, vtlx + vwidth/2, '^');
  mvaddch(vtly+vheight, vtlx + vwidth/2, 'v');
  mvaddch(vtly+vheight/2, vtlx-1, '<');
  mvaddch(vtly+vheight/2, vtlx+vwidth, '>');
  attroff(COLOR_PAIR(GREEN_TEXT));
  for (uint i = 0; i < vheight; i++) {
    mvaddchnstr(i + vtly, vtlx, map[i+tly] + tlx, vwidth);
  }
  attron(COLOR_PAIR(WHITE_TEXT));
  mvaddch(vtly+cy, vtlx+cx, '+');
  mclear(vtlx, vtly+vheight+2, COLS-1, LINES-3);
  Virgate& tile = virgates[tly+cy][tlx+cx];
  mvprintw(vtly+vheight+2, vtlx, tile.eff.name.c_str());
  move(vtly+vheight+3, vtlx);
  for (auto& i : tile.resources) {
    printw("%s: %d ", ge->stuff[i.type].eff.name.c_str(), i.amount);
  } 
  attroff(COLOR_PAIR(WHITE_TEXT));
}

void Province::genRandomForest(Virgate& v) {
  ResourcePair r;
  r.type = randRange(ge->firsts[CC_Wood], ge->lasts[CC_Wood]);
  r.amount = rand() % 100;
  v.resources.push_back(r);
}

void Province::genRandomPlain(Virgate& v) {
  ResourcePair r;
  r.type = randRange(ge->firsts[CC_Tubers], ge->lasts[CC_Tubers]);
  r.amount = rand() % 100;
  v.resources.push_back(r);

}


void Province::genRandomMountain(Virgate& v) {

}

void Province::genRandomRiver(Virgate& v) {
  ResourcePair r;
  r.type = randRange(ge->firsts[CC_Fish], ge->lasts[CC_Fish]);
  uint yield = ge->foods[ r.type - ge->firsts[CC_Fish] ].yield;
  r.amount = randRange(yield/2, yield);
  v.resources.push_back(r);
}

void Province::genRandomBog(Virgate& v) {

}

