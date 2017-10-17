
#include <cstdlib>
#include <ncurses.h>
#include <limits>
#include <cmath>
#include "skirmish.hpp"
#include "util_ncurses.hpp"

void Skirmish::quitMessage() {
  important.push_back(true);
  messages.push_back("Repeat off.");
}

void Skirmish::bloodEffect() {
  move(0, 1);
  auto c = inch();
  int start_x;
  int start_y;
  int width, length;
  int start;
  for (int i = 0; i < 6; i++) {
    width = 0;
    start = 0;
    start_y = rand() % LINES;
    start_x = rand() % COLS;
    length = rand() % 15;
    for (int j = 0; j < length; j++) {
      start += (rand() % 2) - (rand() % 3);
      if (j < 4)
	width += rand() % 2;
      else
	width -= rand() % 2;
      for (int k = 0; k < width; k++) {
	if ( (start_y + j) < LINES && (start_x + start + k) < COLS &&
	     (start_y + j) > 0 && (start_x + start + k) > 0) {
	  move(start_y + j, start_x + start + k);
	  c = inch();
	  attron(COLOR_PAIR(BLOODY_TEXT));
	  addch(c);
	  attroff(COLOR_PAIR(BLOODY_TEXT));
	}
      }
    }
  }
}

void Skirmish::fight(Combatant& c) {
  uint my_roll, enemy_roll;
  Combatant& enemy = *c.opponent;
  if (c.ticks_til_block > 0)
    c.ticks_til_block--;
  if (c.ticks_til_attack > 0)
    c.ticks_til_attack--;
  else {
    if (c.stamina == 0)
      c.ticks_til_attack = c.ticks_between_attack * 3;
    else {
      c.stamina--;
      c.ticks_til_attack = c.ticks_between_attack;
    }
    my_roll = c.strength + (rand() % c.skill);
    if (enemy.ticks_til_block == 0) {
      enemy_roll = enemy.strength + (rand() % enemy.reflex);
      enemy.ticks_til_block = enemy.ticks_between_block;
    }
    else
      enemy_roll = 0;
    if (enemy_roll > my_roll && enemy.stamina > 0) {
      enemy.stamina--;
      important.push_back(false);
      messages.push_back(enemy.name + " blocks an attack.");
      enemy.block_exp_gain += c.total_power_score;
    }
    else {
      enemy.wounds++;
      if (enemy.is_player)
	bloodEffect();
      enemy_roll = rand() % 100;
      if (enemy_roll < enemy.wounds * enemy.down_chance) {
	enemy.is_down = true;
	c.in_combat = false;
	important.push_back(true);
	messages.push_back(enemy.name + " is down!");
	c.attack_exp_gain += enemy.total_power_score;
      }
      else {
	important.push_back(false);
	messages.push_back(enemy.name + " is hit.");
	c.attack_exp_gain += enemy.total_power_score;
      }
    }
  }
}

void Skirmish::setup(uint x, uint y, uint width, uint height) {
  tlx = x;
  tly = y;
  map_width = width;
  map_height = height;
  player_present = true;
  finished = false;
  teams.resize(0);
  messages.reserve(100);
  messages.resize(0);
  important.resize(0);
  eliminated.resize(0);
  important.resize(0);
}

void Skirmish::draw() {
  teams[0][0].is_player = true;
  mclear(tlx, tly, tlx+map_width, tly+map_height+6);
  drawBox(tly, tlx, tly+map_height, tlx+map_width, BORDER_TEXT);
  attron(COLOR_PAIR(DARK_GREEN_TEXT));
  mclear(tlx+1, tly+1, tlx+map_width, tly+map_height);
  attroff(COLOR_PAIR(DARK_GREEN_TEXT));
  for (uint i = 0; i < teams.size(); i++) {
    attron(COLOR_PAIR(colors[i]));
    for (uint j = 0; j < teams[i].size(); j++) {
      Combatant &c = teams[i][j];
      if (c.is_down)
	attron(COLOR_PAIR(BLOODY_TEXT));
      mvaddch(c.y, c.x, c.graphic);
      if (c.is_down)
	attroff(COLOR_PAIR(BLOODY_TEXT));
    }
    attroff(COLOR_PAIR(colors[i]));
  }
  if (player_present && !teams[0][0].is_down) {
    attron(COLOR_PAIR(YELLOW_TEXT));
    mvaddch(teams[0][0].y, teams[0][0].x, teams[0][0].graphic);
    attron(COLOR_PAIR(YELLOW_TEXT));
  }
  drawMessages();
  drawStatus();
}

void Skirmish::drawStatus() {
  uint x = map_width + tlx + 3;
  uint y = 3;
  uint name_w = 7;
  uint stam_w = 6;
  uint name_size, hp;
  attron(A_UNDERLINE);
  attron(COLOR_PAIR(BORDER_TEXT));
  mvprintw(y, x, "NAME   STAM  WOUNDS");
  attroff(COLOR_PAIR(BORDER_TEXT));
  attroff(A_UNDERLINE);
  y++;
  for (int i = 0; i < teams.size(); i++) {
    y++;
    attron(A_BOLD);
    attron(COLOR_PAIR(colors[i]));
    mvprintw(y, x, names[i].c_str());
    attroff(COLOR_PAIR(colors[i]));
    attroff(A_BOLD);
    y++;
    for (int j = 0; j < teams[i].size(); j++ ) {
      move(y, x);
      hline(' ', name_w + stam_w + 6);
      Combatant& me = teams[i][j];
      if (name_w > me.name.size())
	name_size = me.name.size();
      else
	name_size = name_w;
      attron(COLOR_PAIR(WHITE_TEXT));
      mvprintw(y, x, me.name.substr(0, name_size).c_str());
      attroff(COLOR_PAIR(WHITE_TEXT));
      attron(COLOR_PAIR(GREEN_TEXT));
      mvprintw(y, x + name_w, "%d", me.stamina);
      attroff(COLOR_PAIR(GREEN_TEXT));
      attron(COLOR_PAIR(RED_SOLID));
      move(y, x+name_w+stam_w);
      if (me.is_down)
	hp = 0;
      else
	hp = 5 - me.wounds;
      hline(' ', hp);
      attroff(COLOR_PAIR(RED_SOLID));
      y++;
    }
  }
}

void Skirmish::drawMessages() {
  uint message_num = 5;
  uint ind;
  color_pair color;
  uint start_line = tly + map_height + 1;
  if (messages.size() < message_num)
    message_num = messages.size();
  for (uint i = 0; i < message_num; i++) {
    ind = messages.size() - message_num + i;
    if (important[ind])
      color = YELLOW_TEXT;
    else 
      color = BRIGHT_GREY_TEXT;
    attron(COLOR_PAIR(color));
    mvprintw(start_line + i, 3, messages[ind].c_str());
    attroff(COLOR_PAIR(color));
  }
}

void Skirmish::newTeam(Creature& captain, string& name, color_pair color) {
  teams.resize(teams.size() + 1);
  addCombatant(captain);
  names.push_back(name);
  colors.push_back(color);
  eliminated.push_back(false);
}

void Skirmish::newTeam(Person& captain, string& name, color_pair color) {
  teams.resize(teams.size() + 1);
  addCombatant(captain);
  names.push_back(name);
  colors.push_back(color);
  eliminated.push_back(false);
}

void Skirmish::addCombatant(Person& combatant) {
  teams.back().emplace_back();
  Combatant& c = teams.back().back();
  c.name = combatant.eff.name;
  c.stamina = 10 + combatant.skills[SI_cardio];
  c.reflex = combatant.speed + combatant.skills[SI_blocking];
  c.strength = combatant.size + combatant.skills[SI_muscles];
  c.skill = combatant.skills[SI_melee];
  c.down_chance = 20;
  c.ticks_between_attack = 10 - 
    static_cast<int>(log2(combatant.speed + combatant.skills[SI_cardio]));
  c.ticks_between_attack = 10 - 
    static_cast<int>(log2(combatant.speed + combatant.skills[SI_cardio]));

  addCombatant();
}

void Skirmish::addCombatant(Creature& combatant) {
  teams.back().push_back(combatant.combat);
  addCombatant();
}

void Skirmish::addCombatant() {
  Combatant& c = teams.back().back();
  c.graphic = c.name[0];
  c.x = tlx+1+(rand() % (map_width-1));
  c.y = tly+1+(rand() % (map_height-1));
  c.in_combat = false;
  c.is_down = false;
  c.is_dead = false;
  c.wounds = 0;
  c.block_exp_gain = 0;
  c.attack_exp_gain = 0;
  c.ticks_til_attack = c.ticks_between_attack;
  c.ticks_til_block = c.ticks_between_block;
  c.total_power_score = (c.reflex + c.strength + c.stamina) / 
    (c.ticks_between_attack + c.ticks_between_block);
    
}

uint Skirmish::distance(uint x1, uint y1, uint x2, uint y2) {
  uint x_dist = abs(x2 - x1);
  uint y_dist = abs(y2 - y1);
  if (x_dist > y_dist)
    return x_dist;
  else return y_dist;
}

void Skirmish::tick() {

  if (finished)
    return;
  uint flag_val = std::numeric_limits<uint>::max();
  uint shortest_distance;
  uint closest_x;
  uint closest_y;
  uint temp_distance;
  bool member_up;
  uchar teams_remaining = teams.size();;
  uchar last_team_standing;
  Combatant* new_opponent;
  for (uint current_team = 0; current_team < teams.size(); current_team++) {
    if (eliminated[current_team])
      continue;
    member_up = false;
    for (auto& me : teams[current_team]) {
      if (!me.is_down)
	member_up = true;
      shortest_distance = flag_val;
      closest_x = flag_val;
      closest_y = flag_val;
      if (!me.in_combat && !me.is_down) {
	for (uint enemy_team = 0; enemy_team < teams.size(); enemy_team++) {
	  if (current_team == enemy_team)
	    continue;
	  for (auto& enemy : teams[enemy_team]) {
	    if (!enemy.is_down) {
	      temp_distance = distance(me.x, me.y, enemy.x, enemy.y);
	      if (temp_distance < shortest_distance) {
		shortest_distance = temp_distance;
		closest_x = enemy.x;
		closest_y = enemy.y;
		new_opponent = &enemy;
	      }
	    }
	  }
	}
	if (closest_x > me.x)
	  me.x++;
	else if (closest_x < me.x)
	  me.x--;
	if (closest_y > me.y)
	  me.y++;
	else if (closest_y < me.y)
	  me.y--;
	if (shortest_distance < 3) {
	  me.in_combat = true;
	  me.opponent = new_opponent;
	  if (!new_opponent->in_combat) {
	    new_opponent->in_combat = true;
	    new_opponent->opponent = &me;
	  }
	}
      }
      else if (!me.is_down) {
	fight(me);
      }
    }
    if (!member_up) {
      teams_remaining--;
//      cerr<<"eliminated:"<<current_team;
      eliminated[current_team] = true;
    }
    else last_team_standing = current_team;
  }
  if (teams_remaining < 2) {
    finished = true;
//    mvprintw(tly - 1, 3, "game over. length of names: %d", names.size());
    messages.push_back(names[last_team_standing] + " won the battle!" );
    important.push_back(true);
    if (player_present) {
      Combatant& me =teams[0][0];
      if (me.wounds > 0)
	me.healing_days = (rand() % me.wounds);
      else
	me.healing_days = 0;
      if (me.is_down) {
	me.healing_days++;
	me.healing_days *= 2;
      }
      if (me.healing_days == 0)
	messages.push_back(me.name + " has no major wounds." );
      else if (me.healing_days == 1)
	messages.push_back(me.name + " will have to rest for a day." );
      else 
	messages.push_back(me.name + " will have to heal for " + to_string(me.healing_days) + " days." );
      important.push_back(true);
    }
  }
  draw();
}  
