
#include "convo_menu.hpp"
#include "util.hpp"

void ConvoMenu::setNode(ConversationNode& node) {
  selected = 0;
  height = 0;
  items_source.resize(0);
  items.resize(0);
  for (uint i = 0; i < node.responses.size(); i++) {
    items_source.push_back( &(node.responses[i].text) );
  }
}

void ConvoMenu::setup(uint width) {
  items.resize(items_source.size());
  
  for (uint i = 0; i < items_source.size(); i++) {
    stringChopper( *(items_source[i]), items[i], width);
    height += items[i].size();
  }
}

bool ConvoMenu::up() {
  if (selected > 0) {
    selected--;
    return true;
  }
  return false;
}

bool ConvoMenu::down() {
  if (selected < items.size()) {
    selected++;
    return true;
  }
  return false;
}
