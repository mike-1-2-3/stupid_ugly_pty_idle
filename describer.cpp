#include "describer.hpp"
#include "util.hpp"
#include "person.hpp"
#include "province.hpp"
#include "game_engine.hpp"

void Describer::up() {}
void Describer::down() {}

void Describer::describe(Province& province) {
  describe(province.eff);
  print_buffer.push_back("@BPopulations");
  for (uint i = 0; i < ge->sentients.size(); i++)
    print_buffer.push_back(ge->sentients[i].eff.name + ": " + 
			   to_string(province.sentient_populations[i]));
}

void Describer::describe(Person& person) {
  describe(person.eff);
  for (uint i = 0; i < person.skills.size(); i++) {
    print_buffer.push_back( (*skill_names)[i] + ": " + to_string(person.skills[i]) + "  " +
			    to_string(person.exp_progress[i]) + "/" +
			    to_string(person.expToNext(i)) );
  }
    
}

void Describer::describe(Effable& eff) {
  print_buffer.push_back("@B" + eff.name);
  stringChopper(eff.description, print_buffer, line_width - 2);
  for (uint i = 0; i < eff.properties.size(); i++) {
    print_buffer.push_back(eff.properties[i]);
  }

  print_buffer.push_back(" ");
}
