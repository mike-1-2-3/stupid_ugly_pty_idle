#include "game_engine.hpp"
#include "enums/menus.hpp"
#include "enums/misc.hpp"
#include <ncurses.h>


void GameEngine::poll() {
  int input = getch();
  while (input != ERR) {
    currentEventHandler(input);
    input = getch();
  }
}

void GameEngine::mainMenuHandler(int input) {
  switch(input) {
    case (KEY_DOWN) :
      main_menu.down();
      display.clearMenu(main_menu);
      display.drawMenu(main_menu);
      break;
    case (KEY_UP) :
      main_menu.up();
      display.clearMenu(main_menu);
      display.drawMenu(main_menu);
      break;
    case (KEY_RIGHT) :
      setGameState(GS_browse_herald);
      break;
    case (KEY_ENTER) :
    case (10) :
      switch(main_menu.selected) {
	case (MMI_court) :
	  if (player.vacant[PI_Gentlemen_Usher]) {
	      string message = "No Usher - if anyone came to court, they found it empty and left.";
	      display.drawHeadline(timestamp, message);
          }
	  else if (court_events.empty()) {
	    string message = "No matters need attending to. ";
	    uint r = rand() % 2;
	    if (r == 0)
	      message += "Only the usher is in the great hall.";
	    else
	      message += "Might as well go hunting.";
	    display.drawHeadline(timestamp, message);
	  } 
	  else {
	    uint usher_index = player.retinue[PI_Gentlemen_Usher];
	    Person& usher = people[usher_index];
	    cgen.generateCourtEvent(usher, court_events.front());
	    court_events.pop();
	    queued_states.push_back(GS_queued_conversation);
	    queued_conversers.push_back(usher_index);
	    eventReturn();
	  }
	  break;
	case (MMI_retinue) :
	  setGameState(GS_retinue_menu);
	  break;
	case (MMI_hunt) :
	  setGameState(GS_hunt_menu);
	  break;
	case (MMI_mirror) :
	  setGameState(GS_mirror);
	  break;
	case (MMI_maps) :
	  setGameState(GS_maps);
	  break;
	case (MMI_provinces) :
	  setGameState(GS_provinces_menu);
	  break;
	case (MMI_exit) :
	  exit = true;
	  break;
      }
    break;
  }
}

void GameEngine::provinceHandler(int input) {
  switch(input) {
    case (KEY_DOWN) :
      province_menu.down();
      display.clearMenu(province_menu);
      display.drawMenu(province_menu);
      break;
    case (KEY_UP) :
      province_menu.up();
      display.clearMenu(province_menu);
      display.drawMenu(province_menu);
      break;
    case (KEY_RIGHT) :
//      setGameState(GS_browse_herald);
      break;
    case (27) :
      setGameState(GS_main_menu);
      break;
    case (KEY_ENTER) :
    case (10) :
      setGameState(GS_province_category_menu);
      break;
  }
}

void GameEngine::provinceCategoryHandler(int input) {
  switch(input) {
    case (KEY_DOWN) :
      province_category_menu.down();
      display.clearMenu(province_category_menu);
      display.drawMenu(province_category_menu);
      describeSelectedCategory();
      break;
    case (KEY_UP) :
      province_category_menu.up();
      display.clearMenu(province_category_menu);
      display.drawMenu(province_category_menu);
      describeSelectedCategory();
      break;
    case (KEY_RIGHT) :
      setGameState(GS_province_items);
    case (KEY_ENTER) :
    case (10) :
      setGameState(GS_province_items);
      break;
    case (27) :
      setGameState(GS_provinces_menu);
      break;
  }
}

void GameEngine::provinceItemHandler(int input) {
  switch(input) {
    case (KEY_DOWN) :
      province_item_menu.down();
      display.clearMenu(province_item_menu);
      display.drawMenu(province_item_menu);
      break;
    case (KEY_UP) :
      province_item_menu.up();
      display.clearMenu(province_item_menu);
      display.drawMenu(province_item_menu);
      break;
    case (KEY_LEFT) :
      setGameState(GS_province_category_menu);
    case (KEY_ENTER) :
    case (10) :
      break;
    case (27) :
      setGameState(GS_provinces_menu);
      break;
  }
  display.drawSubheading("                      " +misc[MT_supply] + " " + misc[MT_demand] );
}


void GameEngine::retinueMenuHandler(int input) {
  switch(input) {
    case (KEY_DOWN) :
      retinue_menu.down();
      display.clearMenu(retinue_menu);
      display.drawMenu(retinue_menu);
      describeSelectedPosition();
      break;
    case (KEY_UP) :
      retinue_menu.up();
      display.clearMenu(retinue_menu);
      display.drawMenu(retinue_menu);
      describeSelectedPosition();
      break;
    case (KEY_RIGHT) :
//      setGameState(GS_browse_herald);
      break;
    case (27) :
      setGameState(GS_main_menu);
      break;
  }
}

void GameEngine::huntMenuHandler(int input) {
  switch(input) {
    case (KEY_DOWN) :
      hunt_menu.down();
      display.clearMenu(hunt_menu);
      display.drawMenu(hunt_menu);
      describeSelectedHunt();
      break;
    case (KEY_UP) :
      hunt_menu.up();
      display.clearMenu(hunt_menu);
      display.drawMenu(hunt_menu);
      describeSelectedHunt();
      break;
    case ('h') :
      queued_states.push_back(GS_hunt_menu);
      newHunt();
      break;
    case ('r') :
      queued_states.push_back(GS_hunt_menu);
      skirmish.repeat = true;
      newHunt();
      break;
    case (27) :
      setGameState(GS_main_menu);
      break;
  }
}


void GameEngine::heraldHandler(int input) {
  switch(input) {
    case (KEY_LEFT) :
      setGameState(GS_main_menu);
      break;
    case (KEY_UP) :
      if (herald.up())
	display.drawHerald(herald);
      break;
    case (KEY_DOWN) :
      if (herald.down())
	display.drawHerald(herald);
      break;
  }
}

void GameEngine::skirmishHandler(int input) {
  switch(input) {
    case (KEY_ENTER) :
    case (10) : {
      if (skirmish.finished)
	postSkirmish();
      eventReturn();
      break;
    }
    case (27) :
      if (skirmish.repeat) {
	skirmish.repeat = false;
	skirmish.quitMessage();
      }
      break;
  }
}


void GameEngine::conversationTypingHandler(int input) {
  return;
}

void GameEngine::mirrorHandler(int input) {
  switch(input) {
    case (KEY_ENTER) :
    case (10) :
    case (27) :
      setGameState(GS_main_menu);
    break;
  }
}

void GameEngine::mapHandler(int input) {
  switch(input) {
    case (KEY_UP) :
      world.up();
      break;
    case (KEY_DOWN) :
      world.down();
      break;
    case (KEY_LEFT) :
      world.left();
      break;
    case (KEY_RIGHT) :
      world.right();
      break;

    case (27) :
      setGameState(GS_main_menu);
    break;
  }
  
}


void GameEngine::conversationDisplayedHandler(int input) {
  switch(input) {
    case (KEY_ENTER) :
    case (10) :
      if (converser.waiting)
	display.clearMenu(converser.menu);
      converser.enterPressed();
      if (!converser.finished) {
	  display.startTyping(converser);
	  setGameState(GS_conversation_typing);
      }
      else {
	eventReturn();
//	setGameState(previous_states.back());
//	previous_states.pop_back();
      }
      break;
    case (KEY_UP) :
      if (converser.waiting) {
	converser.menu.up();
	display.drawConvoMenu(converser);
      }
      break;
    case (KEY_DOWN) :
      if (converser.waiting) {
	converser.menu.down();
	display.drawConvoMenu(converser);
      }
      break;
    default :
      if (converser.menu.selected == converser.menu.items.size()) {
	if (input == KEY_BACKSPACE || input == KEY_DC || input == 127) {
	  if (!converser.response.empty()) {
	    converser.response.pop_back();
	    display.clearMenu(converser.menu);
	  }
	}
	else if (isascii(input) && isprint(input))
	  converser.response.push_back(input);
      }
      display.drawConvoMenu(converser);
      break;
  }   
  return;
}

