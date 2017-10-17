
#ifndef SIM_GAME_SENTIENT
#define SIM_GAME_SENTIENT

#include "common.hpp"
#include "enums/skills.hpp"
#include "person.hpp"

struct Sentient {
  Effable eff;
  uchar brain_min;
  uchar brain_max;
  uchar magic_chance; // Out of 100, but roll has to succeed more than once
  uchar magic_min;
  uchar magic_max;
  uchar size;
  uchar speed;
  uchar birth_rate; // Chance in 1,000,000 per person per hour
//  vector<uint> skill_aptitudes;
  void makePerson(Person& new_person);
  void makeMaxedPerson(Person& new_person);
};


#endif
