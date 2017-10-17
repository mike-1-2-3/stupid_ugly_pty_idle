#include "common.hpp"
#include "enums/states.hpp"
#include "menu.hpp"
#include "convo_menu.hpp"
#include "enums/colors.hpp"
#include "herald.hpp"
#include "describer.hpp"
#include "conversation.hpp"

class NcursesDisplay {
public:
  NcursesDisplay() {};
  ~NcursesDisplay();
//  int poll();
  void init();
  void update();
  void setTime(string& time);
  void draw1Column();
  void draw2Column();
  void drawSubheading(string heading);
// draw_conversation.cpp
  void drawConvoTitle(string& title, string& p1, string& p2);
  void setup(Conversation& converser);
  void startTyping(Conversation& converser);
  bool nextChar();
  void drawConvoMenu(Conversation& converser);
  void clearMenu(ConvoMenu& menu);
  void drawConvoHistory(Conversation& converser);
  void drawTracking(uint progress);
  void draw1ColumnTitle(string &title);
  void drawLTitle(string& title, bool active = false);
  void drawRTitle(string& title, bool active = false);
  void drawRCommands(vector<string> commands);
  void drawREnter(string& command);
  void drawLMenu(Menu& menu);
  void drawRMenu(Menu& menu);
  void clearMenu(Menu& menu);

  void clearAll();
  void drawMenu(Menu& menu);
  void drawHerald(Herald& herald);
  void setupDescriber(Describer& describer);
  void drawDescriber(Describer& describer);
  void drawHeadline(string& timestamp, string& message, color_pair color = GREY_TEXT);
private:
  void drawCenteredString(string& text, int y, int startx, int endx, color_pair color);
  void drawLAlignedString(string& text, int y, int startx, int endx, color_pair color);
  void drawBottomBar();
  void drawBorder();
  void drawTopBar();
  void drawColumn();
  int column_x_coor;
  string* input_destination;
  uchar x, y; // TODO: wat?
  string up_arrow, down_arrow;
  
  vector<string>* convo_buffer;
  uint convo_i, convo_j, convo_current_line, convo_start_x;
};
