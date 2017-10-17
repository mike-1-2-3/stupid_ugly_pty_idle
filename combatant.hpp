
#ifndef SIM_GAME_COMBATANT
#define SIM_GAME_COMBATANT

#include "common.hpp"

struct Combatant {
  char graphic;
  string name;
  uint x, y;
  bool in_combat;
  bool is_player;
  Combatant* opponent;
  uint total_power_score;

  uint stamina;
  uint wounds;
  uint down_chance;
  bool is_down;
  bool is_dead;
  uint healing_days;
  uint strength;
  uint pierce;
  uint armor;

  uint reflex;
  uint ticks_between_block;
  uint ticks_til_block;

  uint skill;
  uint ticks_between_attack;
  uint ticks_til_attack;

  uint block_exp_gain;
  uint attack_exp_gain;
};

#endif
