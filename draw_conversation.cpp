
#include <ncurses.h>
#include "ncurses_display.hpp"
#include "util.hpp"
#include "util_ncurses.hpp"

void NcursesDisplay::setup(Conversation& converser) {
  converser.line_width = COLS-6;
  converser.line_space = LINES-6;
  convo_current_line = 3;
  converser.history_height = 3;
}


void NcursesDisplay::clearMenu(ConvoMenu& menu) {
  mclear(5, LINES - 6 - menu.height, COLS-3, LINES - 3);
}

void NcursesDisplay::drawConvoMenu(Conversation& converser) {
  uint y = LINES - 6 - converser.menu.height;
  color_pair color = WHITE_TEXT;
  for (uint i = 0; i < converser.menu.items.size(); i++) {
    if (converser.menu.selected == i)
      color = YELLOW_TEXT;
    else color = WHITE_TEXT;
    attron(COLOR_PAIR(color));
    for (uint j = 0; j < converser.menu.items[i].size(); j++) {
      mvprintw(y,5,converser.menu.items[i][j].c_str() );
      y++;
    }
    attroff(COLOR_PAIR(color));
  }
 
  if (converser.menu.selected == converser.menu.items.size())
    color = YELLOW_TEXT;
  else color = WHITE_TEXT;
  drawBox(LINES-6, 5, LINES-4, COLS - 6, color);
  if (!converser.response.empty()) {
    attron(COLOR_PAIR(color));
    mvprintw(LINES - 5, 6, converser.response.c_str() );
    attroff(COLOR_PAIR(color));
  }
}

void NcursesDisplay::drawConvoTitle(string& title, string& p1, string& p2) {
  uint size = title.size() + p1.size() + p2.size();
  uint start_x = ((COLS-2) / 2) - size/2;
  attron(A_BOLD);
  mvprintw(1, start_x, title.c_str(), p1.c_str(), p2.c_str());
  attroff(A_BOLD);
}

void NcursesDisplay::drawConvoHistory(Conversation& converser) {
  uint hist_item_start = converser.history_height;
  uint hist_item_current = converser.history_height;
  bool first_line;
  uint start_x = 2;
  vector<string> item_buffer;
  if (!converser.history.empty())
    for (uint i = converser.history.size(); i > 0;) {
      i--;
      item_buffer.resize(0);
      ConversationNode* node = converser.history[i];
      stringChopper(node->message, item_buffer, COLS-6, node->speaker.size() +3);
      hist_item_start -= item_buffer.size();
      if (hist_item_start > 2) {
	hist_item_current = hist_item_start;
	attron( COLOR_PAIR(node->speaker_color) );
	mvprintw(hist_item_current, 2, node->speaker.c_str() );
	printw(": ");
	attroff( COLOR_PAIR(node->speaker_color) );
	first_line = true;
	for (auto& line : item_buffer) {
	  if (first_line) {
	    first_line = false;
	    start_x = node->speaker.size() + 4;
	  }
	  else 
	    start_x = 2;
	  attron(COLOR_PAIR(BRIGHT_GREY_TEXT));
	  mvprintw(hist_item_current, start_x, line.c_str());
	  attroff(COLOR_PAIR(BRIGHT_GREY_TEXT));
	  hist_item_current++;
	}
      }
      else break;
    }
}

void NcursesDisplay::startTyping(Conversation& converser) {

  mclear(1, 3, COLS-2, LINES-4);
  convo_i = 0;
  convo_start_x = converser.getSpeaker()->size() + 4;
  convo_j = 0;
  convo_buffer = converser.getTypeBuffer();

  if (converser.history_height + convo_buffer->size() + converser.menu_height + 7 > LINES)
    converser.history_height = LINES - 7 - converser.menu_height - convo_buffer->size();
  convo_current_line = converser.history_height;
  drawConvoHistory(converser);
  converser.history_height += convo_buffer->size();

  attron( COLOR_PAIR(converser.getSpeakerColor()) );
  mvprintw(convo_current_line, 2, converser.getSpeaker()->c_str() );
  printw(": ");
  attroff( COLOR_PAIR(converser.getSpeakerColor()) );
}

bool NcursesDisplay::nextChar() {
  if (convo_i == (*convo_buffer)[convo_j].size()) {
    convo_j++;
    convo_current_line++;
    convo_i = 0;
    convo_start_x = 2;
    if (convo_j == convo_buffer->size()) {
//      convo_current_line++;
      return false;
    }
  }
  attron(COLOR_PAIR(BRIGHT_GREY_TEXT));
  mvaddch(convo_current_line, convo_start_x + convo_i, (*convo_buffer)[convo_j][convo_i]);
  attroff(COLOR_PAIR(BRIGHT_GREY_TEXT));
  convo_i++;
  return true;
}
