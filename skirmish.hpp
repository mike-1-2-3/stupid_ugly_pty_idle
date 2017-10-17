
#ifndef SIM_GAME_SKIRMISH
#define SIM_GAME_SKIRMISH

#include "common.hpp"
#include "person.hpp"
#include "combatant.hpp"
#include "creature.hpp"

struct Warband {
  vector<CreatureIndex> creatures;
  vector<uint> people;
};

class Skirmish {
public :
  Skirmish() : finished(false) {};
  void newTeam(Person& captain, string& name, color_pair color);
  void newTeam(Creature& captain, string& name, color_pair color);
  void addCombatant(Person& combatant);
  void addCombatant(Creature& combatant);
  void startBattle();
  void setup(uint x, uint y, uint width, uint height);
  void draw();
  void tick();
  void quitMessage();
  bool finished;
  bool repeat;
  uint tlx, tly, map_height, map_width;
  bool player_present;
  vector<vector<Combatant>> teams;
private :
  void fight(Combatant& c);
  uint distance(uint x1, uint y1, uint x2, uint y2);
  void drawMessages();
  void drawStatus();
  void bloodEffect();
  void addCombatant();
  vector<bool> eliminated;
  vector<string> names;
  vector<color_pair> colors;
  vector<string> messages;
  vector<bool> important;
};

#endif
