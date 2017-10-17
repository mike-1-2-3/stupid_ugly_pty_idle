
#ifndef SIM_GAME_GAME_ENGINE
#define SIM_GAME_GAME_ENGINE

#include <functional>
#include <queue>
#include "common.hpp"
#include "countable.hpp"
#include "enums/states.hpp"
#include "enums/court_events.hpp"
#include "menu.hpp"
#include "herald.hpp"
#include "describer.hpp"
#include "person.hpp"
#include "player.hpp"
#include "position.hpp"
#include "god.hpp"
#include "sentient.hpp"
#include "ncurses_display.hpp"
#include "conversation_generator.hpp"
#include "skirmish.hpp"
#include "world.hpp"
#include "food.hpp"

class GameEngine {
public :
  GameEngine() : cgen(*this) {};
  void init();
  void newGame();

  // =========== Gui Related

  vector<string> headings, misc, harmless_messages, skill_names, property_strings, category_names,
    main_menu_items, land_names, settled_land_names;
  Menu main_menu, retinue_menu, hunt_menu, province_menu, province_category_menu, 
    province_item_menu;
  vector<string> item_strs; 
  Herald herald;
  Describer describer;
  Skirmish skirmish;
  Conversation converser;
  bool animating;

  // ========== Data
  Player player;
  vector<Position> positions;
  vector<Person> people;
  vector<God> gods;
  vector<Creature> monsters;
  vector<Sentient> sentients;
  ConversationGenerator cgen;
  World world;
  vector<Countable> stuff;
  vector<uint> firsts, lasts;
  vector<string> male_names_western;
  vector<Food> foods;


private :
  void setGameState(GameState state);
  GameState state;
  bool exit;
  vector<GameState> queued_states;
  vector<Warband> queued_challengers;
  vector<uint> queued_conversers;
  std::queue<CourtEvent> court_events;
  void eventReturn();
  void postSkirmish();
  void describeSelectedPosition();
  void describeSelectedHunt();
  void describeSelectedProvince();
  void describeSelectedCategory();
  void startConversation(Person &p1, Person &p2);
  void startSkirmish(Warband& challengers);
  void newHunt();
  void addHeraldItem(string& message);

  void poll(); 
  std::function<void(int)> currentEventHandler;
  void mainMenuHandler(int);
  void retinueMenuHandler(int);
  void huntMenuHandler(int);
  void heraldHandler(int);
  void provinceHandler(int);
  void provinceCategoryHandler(int);
  void provinceItemHandler(int);
  void conversationTypingHandler(int);
  void conversationDisplayedHandler(int);
  void skirmishHandler(int);
  void mirrorHandler(int);
  void mapHandler(int);


  // =========== DateTime stuff
  NcursesDisplay display;
  string timestamp;
  uint year;
  uchar month;
  uchar day;
  uchar hour;
  uchar minute;
  uchar second;

  friend class ConversationGenerator;
};

#endif
