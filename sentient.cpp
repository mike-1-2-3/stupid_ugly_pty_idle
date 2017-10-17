
#include "sentient.hpp"
#include "util.hpp"

void Sentient::makePerson(Person& new_person) {
  new_person.brain = randRange(brain_min, brain_max);
  bool magic = true;
  for (uint i = 0; i < 5; i++)
    if ( (rand() % 100) > magic_chance)
      magic = false;
  if (magic)
    new_person.magic = randRange(magic_min, magic_max);
  new_person.size = randRange(size - (size/10), size + (size/10) );
  new_person.speed = randRange(speed - (speed/10), speed + (speed/10) );
  new_person.loyalty = rand() % 100;
  new_person.skills.resize(SI_num);
  new_person.skills.assign(SI_num, 1);
  new_person.exp_progress.resize(SI_num);
  new_person.exp_progress.assign(SI_num, 0);
}

void Sentient::makeMaxedPerson(Person& new_person) {
  new_person.brain = brain_max;
  new_person.size = size + (size/10);
  new_person.magic = magic_max;
  new_person.speed = speed + (speed/10);
  new_person.loyalty = 100;
  new_person.skills.resize(SI_num);
  new_person.skills.assign(SI_num, 1);
  new_person.exp_progress.resize(SI_num);
  new_person.exp_progress.assign(SI_num, 0);

}
