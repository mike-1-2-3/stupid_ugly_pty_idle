#include "person.hpp"
#include <cmath>

void Person::pickName(vector<string>& names) {
  eff.name = names[rand() % names.size()];
}

uint Person::expToNext(uint skill) {
  uint level = skills[skill];
  return pow(level, 4.2) + pow(1.8, level) + level * 10;
}

void Person::addExp(SkillIndex skill, uint amount) {
  ulong next = expToNext(skill);
  ulong total = exp_progress[skill] + amount;
  if (total < next)
    exp_progress[skill] = total;
  else {
    skills[skill]++;
    exp_progress[skill] = total - next;
  }
}

/*
Person::Person() {
  eff.name = "uninit";
  eff.description = "uninit";
  species = 0;
  loyalty = 0;
  intelligence = 0;
  magic = 0;
  strength = 0;
  agility = 0;
}
*/
