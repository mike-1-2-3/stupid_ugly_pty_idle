
#ifndef SIM_GAME_HERALD
#define SIM_GAME_HERALD

#include "enums/colors.hpp"
#include "common.hpp"

struct HeraldItem {
  HeraldItem(string& t, string ts, color_pair c) : text(&t), timestamp(ts), color(c) {};
  string* text;
  string timestamp;
  color_pair color;
};

class Herald {
public:
  Herald();
  void add(string& timestamp, string& message, color_pair color = GREY_TEXT);
  vector<string>* getPrintBuffer(int lines, int line_length);
  bool up();
  bool down();
  bool older_available;
  bool newer_available;
private:
  bool addNext();
  bool atEnd();
  vector<HeraldItem> items;
  vector<string> print_buffer;
  uint max_lines;
  uint line_width;
  uint current_line;
  uint max;
  uint newest;
  int iterator;
  uint offset;
};

#endif
