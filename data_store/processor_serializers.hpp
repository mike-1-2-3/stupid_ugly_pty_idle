
void save(Countable& c) {
  save(c.eff);
  serialize(c.properties);
}

void save(Position& position) {
  save(position.eff);
  serialize(position.salary);
}

void save(Effable& eff) {
  serialize(eff.name, eff.description, eff.properties);
}

void save(Creature& c) {
  save(c.eff);
  save(c.combat);
  serialize(c.spawn_min, c.spawn_max, c.elusiveness);
}

void save(Combatant& c) {
  serialize(c.name, c.graphic, c.stamina, c.down_chance, c.strength, c.pierce, 
	    c.armor, c.reflex, c.ticks_between_block, c.skill, c.ticks_between_attack,
	    c.total_power_score);
}

void save(Sentient& s) {
  save(s.eff);
  serialize(s.brain_min, s.brain_max, s.magic_chance, s.magic_min, s.magic_max, s.size, s.speed);
}

void save(Food& f) {
  serialize(f.nutrients, f.yield);
}


/*
size_t save(Animal& a) {
  serialize(Smaster, a.name, a.weight, a.prey, a.properties);
  serialize(Ssave, a.max_age);
}
*/
