
template<typename T>
void genEnum(const string& enum_name, const string& type, const string& prefix, 
			vector<T> items) {
  vector<string> names;
  for (auto& i : items) {
    names.push_back(i.eff.name);
  }
  genEnum(enum_name, type, prefix, names);
}
/*
template<typename T>
void genEnumMembers(const string& prefix, vector<string> items, ofstream& of) {
  if (items.size() == 0) {
    cout << "empty item list in enum gen. exiting\n";
    exit(0);
  }
  for (auto& item : items)
    for (auto& ch : item) 
      if (ch == ' ')
	ch = '_';
  of << prefix + items[0];
  for (uint i = 1; i < items.size(); i++)
    of << ", " << prefix << items[i];
}

template<typename T>
void genEnumMembers(const string& prefix, vector<T> items, ofstream& of) {
  vector<string> names;
  for (auto& i : items) {
    names.push_back(i.eff.name);
  }
  genEnumMembers(prefix, names, of);
}
*/

//#include "../enums/menus.hpp"
//#include "../enums/misc.hpp"
//vector<string> properties_vec;
vector<string> main_menu, headings;
vector<string> misc, harmless_messages, skill_names, properties, category_names, male_names_western,
  land_names, settled_land_names;
vector<Position> positions;
vector<Creature> creatures;
vector<Sentient> sentients;
vector<Countable> countables;
vector<Food> foods;
vector<uint> firsts, lasts;

void parse_all() {
  parse(main_menu, LANG "/menus/main.txt");
  parse(headings, LANG "/menus/headings.txt");
  parse(misc, LANG "/misc.txt");
  parse(harmless_messages, LANG "/harmless_messages.txt");
  parse(skill_names, LANG "/skills.txt");
  parse(properties, LANG "/properties.txt");
  parse(category_names, LANG "/categories.txt");
  genEnum("CountableCategory", "uchar", "CC_", category_names);
  parse(male_names_western, LANG "/names_male_western.txt");
  parse(land_names, LANG "/land_types.txt");
  genEnum("LandTypeIndex", "uchar", "LTI_", land_names);
  parse(settled_land_names, LANG "/settled_land_types.txt");

  vector<string> position_files = {LANG "/retinue.txt"};
  parse(positions, position_files);
  genEnum("PositionIndex", "uchar", "PI_", positions);

  vector<string> creature_files = {LANG "/huntables.txt"};
  parse(creatures, creature_files);
  genEnum("CreatureIndex", "uchar", "CI_", creatures);

  vector<string> sentient_files = {LANG "/sentients.txt"};
  parse(sentients, sentient_files);
  genEnum("SentientIndex", "uchar", "SEI_", sentients);


  // Countables
  vector<vector<string>> countable_files = 
    {{LANG "/woods.txt"}, 
     {LANG "/fish.txt"}, 
     {LANG "/tubers.txt"}, 
     {LANG "/nutrients.txt"}};
  for (uint i = 0; i < countable_files.size(); i++) {
    firsts.push_back(countables.size());
    parse(countables, countable_files[i]);
    lasts.push_back(countables.size());
  }
  genEnum("CountableIndex", "uint", "CI_", countables);
/*
  std::ofstream of("enums/Countables.hpp", fstream::out);
  of << "// This file was automatically generated, it'll get clobbered when you run process.\n\n";
  of << "#ifndef AUTO_ENUM_Countables\n#define AUTO_ENUM_Countables\n\n";
  of << "enum CountableIndex : uint {\n";

  genEnumMembers("CI_", countables, of);
  */

  vector<string> food_files = {LANG "/fish.txt", LANG "/tubers.txt"};
  parse(foods, food_files);


  DataStore ds;
  ds.open("release/game_data_en.dat", ios::out | ios::binary, Smaster);
  ds.open("release/base_save_en.dat", ios::out | ios::binary, Ssave);
  ds.serialize(Smaster, main_menu, positions, headings, misc, harmless_messages,
               creatures, sentients, skill_names, countables, properties, category_names,
	       male_names_western, foods, land_names, settled_land_names,
	       firsts, lasts);
  cout <<"num names: " << male_names_western.size();
/*
  vector<Animal> animals;
  vector<string> files = {"mammals.txt", "reptiles.txt"};
  parse(properties_vec, "animal_properties.txt");
  parse(animals, files);
  resolve(animals, files);

  cout << animals.size();
  
  for (uint i = 0; i < animals.size(); i++) {
    cout << "Animals after parsing: \n";
    cout << "\"" << animals[i].name << "\"\n";
    cout << "\"" << animals[i].weight << "\"\n";
    cout << "\"" << animals[i].max_age << "\"\n";
    cout << "\"" << animals[i].prey << "\"\n";
//    cout << "\"" << animals[i].properties[0] << "\"\n";
    cout << "size of properties: " << animals[i].properties.size() << "\n";
    cout << "first property: " << (int) animals[i].properties[0] << "\n";
  }

  DataStore ds;
  ds.open(DS_DATA_DIRECTORY "test.dat", ios::out | ios::binary, Smaster);
  ds.open(DS_DATA_DIRECTORY "save.dat", ios::out | ios::binary, Ssave);
  ds.serialize(Smaster, animals);
*/
}

void parse(Food& f) {
  uint num_nutrients = lasts[3] - firsts[3];
  f.nutrients.assign(num_nutrients, 0);
  f.yield = 0;
  collect("yield: ", f.yield);
  for (uint i = 0; i < num_nutrients; i++) {
    collect(countables[firsts[3] + i].eff.name + ": ", f.nutrients[i]);
  }
}

void parse(Position& p) {
  collect("name: ", p.eff.name);
  collect("description: ", p.eff.description);
  collect("salary: ", p.salary);
}

void parse(Countable& c) {
  collect("name: ", c.eff.name);
  collect("description: ", c.eff.description);
  collect("properties: ", c.properties, properties);
}

void parse(Sentient& s) {
  collect("name: ", s.eff.name);
  collect("description: ", s.eff.description);  
  collect("brain_min: ", s.brain_min);
  collect("brain_max: ", s.brain_max);
  collect("magic_chance: ", s.magic_chance);  
  collect("magic_min: ", s.magic_min);
  collect("magic_max: ", s.magic_max);
  collect("size: ", s.size);  
  collect("speed: ", s.speed);  
}

void parse(Creature& c) {
  collect("name: ", c.eff.name);
  collect("name: ", c.combat.name);
  collect("description: ", c.eff.description);
  collect("spawn_min: ", c.spawn_min);
  collect("spawn_max: ", c.spawn_max);
  collect("elusiveness: ", c.elusiveness);
  collect("stamina: ", c.combat.stamina);
  collect("down_chance: ", c.combat.down_chance);
  collect("strength: ", c.combat.strength);
  collect("pierce: ", c.combat.pierce);
  collect("armor: ", c.combat.armor);
  collect("reflex: ", c.combat.reflex);
  collect("ticks_between_attack: ", c.combat.ticks_between_attack);
  collect("ticks_between_block: ", c.combat.ticks_between_block);
  collect("skill: ", c.combat.skill);
  c.combat.total_power_score = (c.combat.reflex + c.combat.strength + c.combat.stamina) / 
    (c.combat.ticks_between_attack + c.combat.ticks_between_block);
  c.eff.properties.push_back("Travels in groups of " + to_string(c.spawn_min) + " to " +
			 to_string(c.spawn_max));
  c.eff.properties.push_back("Difficulty score of " + to_string(c.combat.total_power_score));


}

/*
void parse(Animal& a) {
  collect("name: ", a.name);
  cout << "Collected the name: " << a.name << "\n";
  collect("weight: ", a.weight);
  collect("max age: ", a.max_age);
  collect("properties: ", a.properties, properties_vec);
  a.prey = 0;
}

void resolve(Animal& current, Animal& candidate) {
  test("prey: ", current.prey, candidate.name);
}
*/
