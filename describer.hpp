
#ifndef SIM_GAME_DESCRIBER
#define SIM_GAME_DESCRIBER

#include "common.hpp"

class Person;
class Province;
class GameEngine;

struct Describer {
  Describer() : start_line(0), up_available(false), down_available(false) {};
  void init(GameEngine* game) {ge = game;};
  GameEngine* ge;
  vector<string>* getPrintBuffer() {return &print_buffer;};
  void up();
  void down();
  void describe(Effable& eff);
  void describe(Province& province);
  void describe(Person& person);
  void addTitle(string& str) {print_buffer.push_back("@U" + str);};
  void addString(string& str) {print_buffer.push_back(str);};
  template<typename T> void describe(T& item) {describe(item.eff);};
  void clear() {print_buffer.resize(0);};
  int start_line;
  bool up_available;
  bool down_available;
  int line_width;
  vector<string> print_buffer;
  vector<string>* skill_names;
};

#endif
