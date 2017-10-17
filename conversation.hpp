
#ifndef SIM_GAME_CONVERSATION
#define SIM_GAME_CONVERSATION

#include "conversation_node.hpp"
#include "convo_menu.hpp"

class Conversation {
public :
  void init(vector<ConversationNode>& src);
  void textEntered();
  void responseSelected();
  void enterPressed();
  void typingDone();
  vector<string>* getPrintBuffer();
  string* getSpeaker() { return &((*source)[current_node].speaker);};
  color_pair getSpeakerColor() { return (*source)[current_node].speaker_color;};
  vector<string>* getTypeBuffer();
  vector<ConversationNode>* source;
  vector<ConversationNode*> history;
  vector<ConversationNode> response_history;
  uint current_node;
  string response;
  ConvoMenu menu;
  uint menu_height;
  uint history_height;
  uint current_line;
  uint line_width;
  uint line_space;
  bool waiting;
  bool finished;
private :
  void checkHidden();
  void addResponseToHistory();
  void addEntryToHistory();
  void changeCurrentNode(uint id);
  vector<string> print_buffer;
};

#endif
