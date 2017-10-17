/*
void load(Animal& a) {
  deserialize(Smaster, a.name, a.weight, a.prey, a.properties);
  deserialize(Ssave, a.max_age);
}

size_t save(Animal& a) {
  serialize(Ssave, a.max_age);
}
*/

#include "../enums/menus.hpp"

void load(GameEngine& game) {
  deserialize(Smaster, game.main_menu_items, game.positions, 
	      game.headings, game.misc, game.harmless_messages, game.monsters, game.sentients,
	      game.skill_names, game.stuff, game.property_strings, game.category_names, 
	      game.male_names_western, game.foods, game.land_names,
	      game.settled_land_names, game.firsts, game.lasts);
}

void load(Countable& c) {
  load(c.eff);
  deserialize(c.properties);
}

void load(Food& f) {
  deserialize(f.nutrients, f.yield);
}

void load(Sentient& s) {
  load(s.eff);
  deserialize(s.brain_min, s.brain_max, s.magic_chance, s.magic_min, s.magic_max, s.size, s.speed);
}

void load(Position& position) {
  load(position.eff);
  deserialize(position.salary);
}

void load(Creature& c) {
  load(c.eff);
  load(c.combat);
  deserialize(c.spawn_min, c.spawn_max, c.elusiveness);
}

void load(Combatant& c) {
  deserialize(c.name, c.graphic, c.stamina, c.down_chance, c.strength, c.pierce, 
	      c.armor, c.reflex, c.ticks_between_block, c.skill, c.ticks_between_attack,
	      c.total_power_score);
}

void load(Effable& eff) {
  deserialize(eff.name, eff.description, eff.properties);
}
