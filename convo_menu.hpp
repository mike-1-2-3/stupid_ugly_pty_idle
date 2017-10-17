#ifndef SIM_GAME_CONVO_MENU
#define SIM_GAME_CONVO_MENU

#include "common.hpp"
#include "conversation_node.hpp"

struct ConvoMenu {
  void setNode(ConversationNode& node);
  bool up();
  bool down();
  void setup(uint width);
  vector<string*> items_source;
  vector<vector<string>> items;
  uint selected;
  uint height;
  uint line_width;
};

#endif
