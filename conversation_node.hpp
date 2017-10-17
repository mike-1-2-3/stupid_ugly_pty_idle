
#ifndef SIM_GAME_CONVERSATION_NODE
#define SIM_GAME_CONVERSATION_NODE

#include <functional>
#include "common.hpp"
#include "enums/colors.hpp"


struct EntryPair {
  // Each of the strings in one vector is ORed, each of the vectors are ANDed
  vector<vector<string>> search_terms;
  uint next_node;
  bool effect;
  std::function<void(void)> effect_function;
};

struct ResponsePair {
  string text;
  uint next_node;
  bool effect;
  std::function<void(void)> effect_function;
};

struct ConversationNode {
  string speaker;
  color_pair speaker_color;
  string message;
  vector<ResponsePair> responses;
  vector<EntryPair> hidden_responses;
  bool generic_entries;
  bool no_response;
  uint no_response_next;
  bool terminate;
};

#endif
