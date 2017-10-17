#include "game_engine.hpp"
#include <chrono>
#include <thread>
#include <cstdlib>
#include <time.h>
#include "enums/menus.hpp"
#include "enums/misc.hpp"
#include "enums/SentientIndex.hpp"

void GameEngine::init() {
  exit = false;
  year = 384;
  month = 2;
  day = 7;
  hour = 0;
  minute = 0;
  second = 0;
  animating = false;
  srand(time(NULL));

  for (auto& c : stuff) {
    cerr << c.eff.name;
    for (uint i = 0; i< c.properties.size(); i++)
      cerr << c.properties[i];
    cerr << "\n";
  }

  Province::ge = this;
  describer.init(this);

  world.provinces.resize(1);
  world.provinces[0].eff.name = "Maredelus";
  world.provinces[0].eff.description = "Your home province.";
  world.provinces[0].init(75, 31);

  province_menu.selectAll(world.provinces);
  main_menu.selectAll(main_menu_items);
  retinue_menu.selectAll(positions);
  hunt_menu.selectAll(monsters);
  province_category_menu.selectAll(category_names);
  describer.skill_names = &skill_names;

  people.resize(4);
  sentients[SEI_Reridraromderarum].makeMaxedPerson(people[0]);
  sentients[SEI_Reridraromderarum].makePerson(people[1]);
  sentients[SEI_Reridraromderarum].makePerson(people[3]);
  people[0].eff.name = "You";
  people[1].eff.name = "Gethin";
  people[1].eff.description = "Your most loyal servant.";
  people[2].eff.name = "Caer Ibormeith";
  people[3].pickName(male_names_western);
  player.init(positions.size(), &world, this);
  player.assignRetinue(PI_Grand_Panetier, 1);
  player.assignRetinue(PI_Gentlemen_Usher, 3);
  court_events.push(CE_land_request);

//  gods.resize(1);
//  gods[0].personality.eff.name = "Caer Ibormeith";
  cgen.generateCharGen(people[2]);
  cgen.generateGethinRescue(people[1]);
/*
  queued_states.push_back(GS_main_menu);
  queued_states.push_back(GS_queued_conversation);
  queued_states.push_back(GS_queued_skirmish);
  queued_states.push_back(GS_queued_conversation);

  queued_conversers.push_back(1);
  queued_challengers.emplace_back();
  queued_challengers.back().creatures.push_back(CI_bandit);
  queued_challengers.back().creatures.push_back(CI_bandit);
  queued_conversers.push_back(2);
  skirmish.repeat = false;
*/
  cout << "initing display...\n";
  display.init();

  setGameState(GS_main_menu);

//  display.setup(converser);
//  converser.init(gods[0].personality.convos);
//  startConversation(people[0], gods[0].personality);

  timestamp = to_string(year) + "/" + to_string(month) + "/" + to_string(day) + " "
    + to_string(hour);
  display.setTime(timestamp);
  display.update();
//  eventReturn();


  while (!exit) {

    std::this_thread::sleep_for(std::chrono::milliseconds(34));
    poll();
    second++;
    if (animating) {
      if (state == GS_conversation_typing)
	if (!display.nextChar()) {
	  setGameState(GS_conversation_displayed);
	}
    }
    if (second == 5) {
      if (state == GS_skirmish) {
	skirmish.tick();
	if (skirmish.finished && skirmish.repeat) {
	  postSkirmish();
	  newHunt();
	}
      }
      second = 0;
      minute++;
      if (minute == 60) {
	minute = 0;
	hour++;
	timestamp = to_string(year) + "/" + to_string(month) + "/" + to_string(day) + " "
	  + to_string(hour);
	display.setTime(timestamp);
	if (hour == 24) {
	  addHeraldItem(harmless_messages[rand() % harmless_messages.size()]);
	  hour = 0;
	  day++;
	  if (day == 30) {
	    day = 0;
	    month++;
	    if (month == 12) {
	      month = 0;
	      year++;
	    }
	  }
	}
      }
    }
    display.update();
  }
}

