#include "game_engine.hpp"
#include "enums/menus.hpp"
#include "enums/misc.hpp"
#include "enums/colors.hpp"
#include "enums/CountableCategory.hpp"
#include "util.hpp"

void GameEngine::newGame() {

}

void GameEngine::addHeraldItem(string& message) {
  herald.add(timestamp, message);
  display.drawHeadline(timestamp, message);
  if (state == GS_main_menu || state == GS_browse_herald)
    display.drawHerald(herald);
}

void GameEngine::startConversation(Person &p1, Person &p2) {
  converser.init(p2.convos);
  display.clearAll();
  display.draw1Column();
  display.drawConvoTitle(headings[HI_conversation], p1.eff.name, p2.eff.name);
  display.setup(converser);
  display.startTyping(converser);
  setGameState(GS_conversation_typing);
  cerr << "should set time now: " << timestamp;
  display.setTime(timestamp);
}

void GameEngine::newHunt() {
  queued_states.push_back(GS_queued_skirmish);
  queued_challengers.emplace_back();
  uint num_enemy = randRange( monsters[hunt_menu.selected].spawn_min, 
			 monsters[hunt_menu.selected].spawn_max );
  for (uint i = 0; i < num_enemy; i++)
    queued_challengers.back().creatures.push_back( (CreatureIndex)hunt_menu.selected);
  eventReturn();
}

void GameEngine::startSkirmish(Warband& challengers) {
  display.clearAll();
  display.setTime(timestamp);

  skirmish.setup(1, 4, 40, 20);  

  string st1 = "Your Expedition";
  string st2 = monsters[challengers.creatures[0]].combat.name+"s";
  string st3 = "Combat between %s and %s";
  skirmish.newTeam(people[0], st1, CYAN_TEXT);
  skirmish.addCombatant(people[1]);

  uint p = 0;
  uint m = 0;
  if (!challengers.people.empty()) {
    skirmish.newTeam(people[challengers.people[0]], st2, BRIGHT_GREY_TEXT);
    p++;
  }
  else if (!challengers.creatures.empty()) {
    skirmish.newTeam(monsters[challengers.creatures[0]],st2 , BRIGHT_GREY_TEXT);
    m++;
  }
  else {
    cout << "error - empty warband\n";
//    exit(0);
  }

  while (p < challengers.people.size()) {
    skirmish.addCombatant(people[challengers.people[p]]);
    p++;
  } 
  while (m < challengers.creatures.size()) {
    skirmish.addCombatant(monsters[challengers.creatures[m]]);
    m++;
  } 

  display.draw1Column();
  display.drawConvoTitle(st3, st1, st2);
  setGameState(GS_skirmish);
}

// Should probably be renamed nextEvent() at some point
void GameEngine::eventReturn() {
  if (queued_states.empty()) {
    setGameState(GS_main_menu);
    return;
  }
  else if (queued_states.back() == GS_queued_conversation) {
    startConversation(people[0], people[queued_conversers.back()]);
    queued_conversers.pop_back();
  }
  else if (queued_states.back() == GS_queued_skirmish) {
    startSkirmish(queued_challengers.back());
    queued_challengers.pop_back();
  }
  else 
    setGameState(queued_states.back());
  queued_states.pop_back();
}

void GameEngine::postSkirmish() {
  people[0].addExp(SI_blocking, skirmish.teams[0][0].block_exp_gain);
  people[0].addExp(SI_melee, skirmish.teams[0][0].attack_exp_gain);
}

void GameEngine::describeSelectedPosition() {
  uint selected = retinue_menu.selected;
  describer.clear();
  display.setupDescriber(describer);
  describer.addTitle(misc[MT_position]);
  describer.describe(positions[selected]);
  describer.addTitle(misc[MT_appointee]);
  if (player.vacant[selected])
    describer.addString(misc[MT_vacant]);
  else 
    describer.describe(people[player.retinue[selected]]);
  display.drawDescriber(describer);
  if (!player.vacant[selected])
    display.drawRCommands( {misc[MT_converse], misc[MT_dismiss]} );
}

void GameEngine::describeSelectedHunt() {
  uint selected = hunt_menu.selected;
  describer.clear();
  display.setupDescriber(describer);
  describer.describe(monsters[selected].eff);
  display.drawDescriber(describer);
  display.drawRCommands( {misc[MT_hunt], misc[MT_repeat]} );
}

void GameEngine::describeSelectedProvince() {
  uint selected = province_menu.selected;
  describer.clear();
  display.setupDescriber(describer);
  describer.describe(world.provinces[selected]);
  display.drawDescriber(describer);
  display.drawREnter(misc[MT_details]);
}

void GameEngine::describeSelectedCategory() {
  vector<uint>& supply = world.provinces[province_menu.selected].supply;
  vector<uint>& demand = world.provinces[province_menu.selected].demand;
  uint selected = province_category_menu.selected;
  item_strs.resize(0);
  province_item_menu.items.resize(0);
  for (uint i = firsts[selected]; i < lasts[selected]; i++) {
    string empty = "                      ";
    string empty2 = "       ";
    string &name = stuff[i].eff.name;
    for (uint j = 0; j < name.size() && j < empty.size(); j++) {
      empty[j] = name[j];
    }
    string num = to_string(supply[i]);
    for (uint j = 0; j < num.size() && j < empty2.size(); j++)
      empty2[j] = num[j];
    
    item_strs.push_back(empty + empty2 + to_string(demand[i]));
  }
  province_item_menu.selectAll(item_strs);
  display.drawSubheading("                      " +misc[MT_supply] + " " + misc[MT_demand] );
  display.drawRMenu(province_item_menu);
}


void GameEngine::setGameState(GameState new_state) {
  switch (new_state) {
    case GS_main_menu :
      display.clearAll();
      display.setTime(timestamp);
      display.draw2Column();
      display.drawLTitle(headings[HI_hall], true);
      display.drawRTitle(headings[HI_herald]);
      main_menu.focus = true;
      display.drawLMenu(main_menu);
      display.drawHerald(herald);
      currentEventHandler = [this](int n){mainMenuHandler(n);};
      break;
    case GS_browse_herald :
      display.clearAll();
      display.setTime(timestamp);
      display.draw2Column();
      display.drawHerald(herald);
      display.drawLTitle(headings[HI_hall]);
      display.drawRTitle(headings[HI_herald], true);
      main_menu.focus = false;
      display.drawLMenu(main_menu);
      currentEventHandler = [this](int n){heraldHandler(n);};
      break;
    case GS_retinue_menu :
      display.clearAll();
      display.setTime(timestamp);
      display.draw2Column();
      display.drawLTitle(headings[HI_position], true);
      display.drawRTitle(headings[HI_courtier]);
      retinue_menu.focus = true;
      display.drawLMenu(retinue_menu);
      describeSelectedPosition();
      currentEventHandler = [this](int n){retinueMenuHandler(n);};
      break;
    case GS_hunt_menu :
      display.clearAll();
      display.setTime(timestamp);
      display.draw2Column();
      display.drawLTitle(headings[HI_target], true);
      display.drawRTitle(headings[HI_courtier]);
      hunt_menu.focus = true;
      display.drawLMenu(hunt_menu);
      describeSelectedHunt();
      currentEventHandler = [this](int n){huntMenuHandler(n);};      
      break;
    case GS_conversation_typing :
      animating = true;
      currentEventHandler = [this](int n){conversationTypingHandler(n);};
      break;
    case GS_conversation_displayed :
      converser.typingDone();
      if (converser.waiting) {
	display.drawConvoMenu(converser);
      }
      animating = false;
      currentEventHandler = [this](int n){conversationDisplayedHandler(n);};
      break;
    case GS_skirmish :
      currentEventHandler = [this](int n){skirmishHandler(n);};
      break;
    case GS_mirror :
      display.clearAll();
      display.setTime(timestamp);
      display.draw2Column();
      display.drawLTitle(headings[HI_target], true);
      display.drawRTitle(headings[HI_courtier]);
      currentEventHandler = [this](int n){mirrorHandler(n);};      
      describer.clear();
      display.setupDescriber(describer);
      describer.describe(people[0]);
      display.drawDescriber(describer);
      break;
    case GS_maps :
      display.clearAll();
      display.setTime(timestamp);
      display.draw1Column();
      display.draw1ColumnTitle(main_menu_items[MMI_maps]);
      currentEventHandler = [this](int n){mapHandler(n);};
      world.draw();
      break;
    case GS_provinces_menu :
      display.clearAll();
      display.setTime(timestamp);
      display.draw2Column();
      display.drawLTitle(headings[HI_provinces], true);
      display.drawRTitle(headings[HI_description]);
      province_menu.focus = true;
      display.drawLMenu(province_menu);
      describeSelectedProvince();
      currentEventHandler = [this](int n){provinceHandler(n);};
      break;
    case GS_province_category_menu :
      display.clearAll();
      display.setTime(timestamp);
      display.draw2Column();
      display.drawLTitle(headings[HI_categories], true);
      display.drawRTitle(headings[HI_items]);
      province_category_menu.focus = true;
      province_item_menu.focus = false;
      display.drawLMenu(province_category_menu);
      describeSelectedCategory();
      currentEventHandler = [this](int n){provinceCategoryHandler(n);};
      break;
    case GS_province_items :
      display.clearAll();
      display.setTime(timestamp);
      display.draw2Column();
      display.drawLTitle(headings[HI_categories]);
      display.drawRTitle(headings[HI_items], true);
      province_category_menu.focus = false;
      display.drawLMenu(province_category_menu);
      display.drawMenu(province_item_menu);
      province_item_menu.focus = true;
      display.drawSubheading("                      " +misc[MT_supply] + " " + misc[MT_demand] );
      currentEventHandler = [this](int n){provinceItemHandler(n);};
      break;
  }

  state = new_state;
}

