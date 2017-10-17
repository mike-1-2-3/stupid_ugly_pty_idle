#include "ncurses_display.hpp"
#include <ncurses.h>
#include "util_ncurses.hpp"

NcursesDisplay::~NcursesDisplay() {
  endwin();
}

void NcursesDisplay::init() {
  initscr();
  if(has_colors() == FALSE){
    endwin();
    //TODO:TRANSLATE
    cout << "Your terminal does not support color, please complain to the dev in a Github issue. Exiting. \n";
    exit(0);
  }
  if (LINES < 35 || COLS < 60) {
    endwin();
    cout << "Terminal size of " << LINES << " lines and " << COLS << " columns is too small.\n";
    cout << "I use 38 lines and 110 columns to develop, this size or larger is recommended.\n";
    cout << "A big font size is also nice, I use 16 point Liberation Mono.\n";
    cout << "If a bigger terminal is not available, please complain to the dev in a Github issue.\n";
    exit(0);
  }
  start_color();
  init_color(MC_BLUE, 100, 100, 700); 
  init_color(MC_BROWN, 800, 700, 500); 
  init_color(MC_GREY, 500, 500, 500); 
  init_color(MC_BRIGHT_GREY, 800, 800, 800); 
  init_color(MC_WHITE, 1000, 1000, 1000); 
  init_color(MC_PINK, 1000, 700, 700);
  init_color(MC_BRIGHT_RED, 800, 200, 200);
  init_color(MC_YELLOW, 900, 900, 200);
  init_color(MC_GOLD, 700, 700, 200);
  init_color(MC_BLOOD, 500, 100, 100);
  init_color(MC_DARK_GREEN, 10, 100, 10);
  init_color(MC_FOREST, 50, 300, 100);
  init_pair(FOREST_SOLID, MC_DARK_GREEN, MC_FOREST);
  init_pair(DARK_GREEN_TEXT, COLOR_GREEN, MC_DARK_GREEN);
  init_pair(BLOODY_TEXT, MC_PINK, MC_BLOOD);
  init_pair(BORDER_TEXT, MC_BROWN, COLOR_BLACK);
  init_pair(WHITE_TEXT, MC_WHITE, COLOR_BLACK);
  init_pair(GREY_TEXT, MC_GREY, COLOR_BLACK);
  init_pair(CYAN_TEXT, COLOR_CYAN, COLOR_BLACK);
  init_pair(GREEN_TEXT, COLOR_GREEN, COLOR_BLACK);
  init_pair(PINK_TEXT, MC_PINK, COLOR_BLACK);
  init_pair(YELLOW_TEXT, MC_YELLOW, COLOR_BLACK);
  init_pair(RED_SOLID, MC_BRIGHT_RED, MC_BRIGHT_RED);
  init_pair(BLUE_SOLID, MC_BLUE, MC_BLUE);
  init_pair(GREEN_SOLID, COLOR_GREEN, COLOR_GREEN);
  init_pair(BRIGHT_GREY_TEXT, MC_BRIGHT_GREY, COLOR_BLACK);
  init_pair(GREY_SOLID, MC_BRIGHT_GREY, MC_GREY);
  init_pair(GOLD_SOLID, MC_YELLOW, MC_GOLD);
  noecho();
  curs_set(0);
  nodelay(stdscr, TRUE);
  ESCDELAY = 0;
  keypad(stdscr, TRUE);
  up_arrow = "^";
  down_arrow = "v";
  convo_i = 0;
  convo_j = 0;
}

void NcursesDisplay::clearAll() {
  clear();
}

void NcursesDisplay::drawSubheading(string heading) {
  attron(COLOR_PAIR(BORDER_TEXT));
  mvprintw(3, column_x_coor + 1, heading.c_str());
  attroff(COLOR_PAIR(BORDER_TEXT));
}

void NcursesDisplay::draw1Column() {
  drawBorder();
  drawBottomBar();
  drawTopBar();
}

void NcursesDisplay::draw2Column() {
  draw1Column();
  column_x_coor = COLS / 3;
  drawColumn();
} 
/*
int NcursesDisplay::poll() {
  int input = getch();
  while (input != ERR) {
    if (string_input_mode == true)
      pushChar(input);
    //addch('a');
    input = getch();
  }
}
*/
void NcursesDisplay::update() {
  refresh();
}

// =========== Drawing ==========

void NcursesDisplay::drawTracking(uint progress) {
  move(5, 5);
  printw("Tracking Progress:");
  move(6, 5);
  attron(COLOR_PAIR(BLUE_SOLID));
  hline(' ', progress);
  attroff(COLOR_PAIR(BLUE_SOLID));
}

void NcursesDisplay::drawHeadline(string& timestamp, string& message, color_pair color) {
  string tmp = timestamp + " " + message;
  int start_x = 16;
  int end_x = COLS - 2;
  int y = LINES - 2;
  mclear(start_x, y, end_x+1, y+1);
  drawLAlignedString(tmp, y, start_x, end_x, color);
}


void NcursesDisplay::drawRCommands(vector<string> commands) {
  int y = LINES-4;
  int x = column_x_coor+1;
  
  move(y,x);
  for (uint i = 0; i < commands.size(); i++) {
    attron(COLOR_PAIR(GREEN_TEXT));
    addch(commands[i][0]);
    attroff(COLOR_PAIR(GREEN_TEXT));
    attron(COLOR_PAIR(WHITE_TEXT));
    printw(commands[i].substr(1).c_str());
    addch(' ');
    attroff(COLOR_PAIR(WHITE_TEXT));
  }
}

void NcursesDisplay::drawREnter(string& command) {
  int y = LINES-4;
  int x = column_x_coor+1;  
  move(y,x);
  attron(COLOR_PAIR(GREEN_TEXT));
  printw("Enter ");
  attroff(COLOR_PAIR(GREEN_TEXT));
  attron(COLOR_PAIR(WHITE_TEXT));
  printw(command.c_str());
  attroff(COLOR_PAIR(WHITE_TEXT));
}

void NcursesDisplay::setupDescriber(Describer& describer) {
  describer.line_width = COLS-1 - column_x_coor+1;
}

void NcursesDisplay::drawDescriber(Describer& describer) {
  int start_y = 4;
  int end_y = LINES-5;
  int start_x = column_x_coor+1;
  int end_x = COLS-1;  
  mclear(start_x, start_y, end_x, end_y+2);
  int line_space = end_y - start_y;
  int current_line = start_y;
  string pgup = "^";
  string pgdn = "v";
  string tmp;
  vector<string>* buffer = describer.getPrintBuffer();
  uint end_index = describer.start_line + line_space;
  for (uint i = describer.start_line; i < end_index && i < buffer->size(); i++) {
    if ( (*buffer)[i][0] == '@') {
      if ( (*buffer)[i][1] == 'U') {
	tmp = (*buffer)[i].substr(2);
	attron(A_UNDERLINE);
	drawCenteredString(tmp, current_line, start_x, end_x, WHITE_TEXT);
	attroff(A_UNDERLINE);
      }
      else if ( (*buffer)[i][1] == 'B') {
	attron(A_BOLD);
	mvprintw(current_line, start_x, (*buffer)[i].substr(2).c_str());
	attroff(A_BOLD);
      }
    }
    else mvprintw(current_line, start_x, (*buffer)[i].c_str());

    current_line++;
  }
  color_pair color = GREY_TEXT;
  if (describer.up_available)
    color = GREEN_TEXT;
  drawCenteredString(pgup, 3, start_x, end_x, color);
  if (describer.down_available)
    color = GREEN_TEXT;
  else color = GREY_TEXT;
  drawCenteredString(pgdn, LINES-4, start_x, end_x, color);
}

void NcursesDisplay::drawHerald(Herald& herald) {
  int start_y = 4;
  int end_y = LINES-4;
  int start_x = column_x_coor+1;
  int end_x = COLS-2;  
  mclear(start_x, start_y, end_x, end_y+1);
  int line_space = end_y - start_y;
  int line_width = end_x - start_x;
  int current_line = start_y;
  string pgup = "^";
  string pgdn = "v";
  vector<string>* buffer = herald.getPrintBuffer(line_space-1, line_width);
  for (int i = buffer->size() - 1; i >=  0; i--) {
    mvprintw(current_line, start_x, (*buffer)[i].c_str());
    current_line++;
  }
  color_pair color = GREY_TEXT;
  if (herald.older_available)
    color = GREEN_TEXT;
  drawCenteredString(pgup, 3, start_x, end_x, color);
  if (herald.newer_available)
    color = GREEN_TEXT;
  else color = GREY_TEXT;
  drawCenteredString(pgdn, LINES-4, start_x, end_x, color);
} 

void NcursesDisplay::drawCenteredString(string& text, int y, int startx, int endx, color_pair color) {
  attron(COLOR_PAIR(color));
  int size = text.size();
  int space = endx - startx + 1;

  if (size == space)
    mvprintw(y, startx, text.c_str() );
  else if (size > space) {
    mvprintw(y, startx, text.substr(0, space).c_str() );
    move(y, startx + space - 1); addch('-');
  }
  // If it needs to be centered
  if (size < space) {
    int c_startx = (space / 2 - size / 2) + startx;
    mvprintw(y, c_startx, text.c_str() );
  }
  attroff(COLOR_PAIR(color));
}

void NcursesDisplay::drawLAlignedString(string& text, int y, int startx, int endx, color_pair color) {

  int size = text.size();
  int space = endx - startx + 1;

  attron(COLOR_PAIR(color));
  if (size == space || size < space)
    mvprintw(y, startx, text.c_str() );
  else if (size > space) {
    mvprintw(y, startx, text.substr(0, space).c_str() );
    move(y, startx + space - 1); addch('-');
  }
  attroff(COLOR_PAIR(color));
}

void NcursesDisplay::draw1ColumnTitle(string& title) {
  attron(A_BOLD);
  drawCenteredString(title, 1, 1, COLS, WHITE_TEXT);  
  attroff(A_BOLD);
}
void NcursesDisplay::drawLTitle(string& title, bool active) {
  color_pair color = GREY_TEXT;
  if (active)
    color = WHITE_TEXT;
  for (int i = 1; i < column_x_coor-1; i++)
    mvaddch(1, i, ' ');
  attron(A_BOLD);
  drawCenteredString(title, 1, 1, column_x_coor-1, color);
  attroff(A_BOLD);
}

void NcursesDisplay::drawRTitle(string& title, bool active) {
  color_pair color = GREY_TEXT;
  if (active)
    color = WHITE_TEXT;
  for (int i = column_x_coor+1; i < COLS-1; i++)
    mvaddch(1, i, ' ');
  attron(A_BOLD);
  drawCenteredString(title, 1, column_x_coor+1, COLS-1, color);
  attroff(A_BOLD);
}

void NcursesDisplay::drawLMenu(Menu& menu) {
  menu.start_y = 3;
  menu.end_y = LINES-4;
  menu.start_x = 2;
  menu.end_x = column_x_coor-1;
  drawMenu(menu);
}

void NcursesDisplay::drawRMenu(Menu& menu) {
  menu.start_y = 4;
  menu.end_y = LINES-4;
  menu.start_x = column_x_coor+1;
  menu.end_x = COLS-2;
  drawMenu(menu);
}

void NcursesDisplay::clearMenu(Menu& menu) {
  for (int i = menu.start_x; i < menu.end_x; i++)
    for (int j = menu.start_y; j < menu.end_y; j++)
      mvaddch(j, i, ' ');
}

void NcursesDisplay::drawMenu(Menu& menu) {
  clearMenu(menu);
  int current_y = menu.start_y;
  int item_space = menu.end_y - menu.start_y + 1;
  color_pair color = GREEN_TEXT;

  if (menu.first == 0)
    color = GREY_TEXT;
  drawCenteredString(up_arrow, current_y, menu.start_x, menu.end_x, color);
  current_y++;
  uint i = menu.first;

  while (i < menu.items.size() && current_y < item_space) {
    if (menu.focus == true && i == menu.selected) {
      attron(A_STANDOUT);
      drawLAlignedString(*((menu.items)[i]), current_y, menu.start_x, menu.end_x, WHITE_TEXT);
      attroff(A_STANDOUT);
    }
    else drawLAlignedString(*(menu.items)[i], current_y, menu.start_x, menu.end_x, WHITE_TEXT);
    i++;
    current_y++;
  }

  color = GREY_TEXT;
  if (i < menu.items.size())
    color = GREEN_TEXT;
  drawCenteredString(down_arrow, menu.end_y, menu.start_x, menu.end_x, color);
  menu.last_visible = i - 1;
}

void NcursesDisplay::drawColumn() {
  attron(COLOR_PAIR(BORDER_TEXT));
  move(1, column_x_coor);         vline(ACS_VLINE, LINES-3);
  move(1, column_x_coor);         addch(ACS_TTEE);
  move(LINES-3, column_x_coor);   addch(ACS_BTEE);

  move(1, column_x_coor);  addch(ACS_VLINE);
  move(0, column_x_coor);  addch(ACS_TTEE);
  move(2, column_x_coor);  addch(ACS_PLUS);
  attroff(COLOR_PAIR(BORDER_TEXT));
}

void NcursesDisplay::drawBorder() {
  attron(COLOR_PAIR(BORDER_TEXT));
  // Lines
  move(0,0);       hline(ACS_HLINE, COLS);
  move(LINES-1,0); hline(ACS_HLINE, COLS);
  move(0,0);       vline(ACS_VLINE, LINES);
  move(0,COLS-1);  vline(ACS_VLINE, LINES);

  // Corners
  move(0,0);             addch(ACS_ULCORNER);
  move(LINES-1,0);       addch(ACS_LLCORNER);
  move(0, COLS-1);       addch(ACS_URCORNER);
  move(LINES-1, COLS-1); addch(ACS_LRCORNER);
  attroff(COLOR_PAIR(BORDER_TEXT));

//  chtype test = 'I' | COLOR_PAIR(BLOODY_TEXT);
//  mvaddch(0, 0, test);


}

void NcursesDisplay::drawBottomBar() {
  attron(COLOR_PAIR(BORDER_TEXT));
  // Line above date, intersection with main border
  move(LINES-3, 1);         hline(ACS_HLINE, COLS - 2);
  move(LINES-3, 0);         addch(ACS_LTEE);
  move(LINES-3, COLS - 1);  addch(ACS_RTEE);

  // Separator between date and notification
  move(LINES-2, 14);  addch(ACS_VLINE);
  move(LINES-1, 14);  addch(ACS_BTEE);
  move(LINES-3, 14);  addch(ACS_TTEE);

  attroff(COLOR_PAIR(BORDER_TEXT));
}

void NcursesDisplay::drawTopBar() {
  attron(COLOR_PAIR(BORDER_TEXT));
  move(2, 1);         hline(ACS_HLINE, COLS - 2);
  move(2, 0);         addch(ACS_LTEE);
  move(2, COLS - 1);  addch(ACS_RTEE);
  attroff(COLOR_PAIR(BORDER_TEXT));
}

void NcursesDisplay::setTime(string& time) {
  attron(COLOR_PAIR(WHITE_TEXT));
  mvprintw(LINES-2, 1, "             ");
  mvprintw(LINES-2, 1, time.c_str());
  attroff(COLOR_PAIR(WHITE_TEXT));
}

