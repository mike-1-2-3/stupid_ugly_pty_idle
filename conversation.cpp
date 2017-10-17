#include "conversation.hpp"
#include "util.hpp"
#include "enums/colors.hpp"
#include <cctype>

void Conversation::init(vector<ConversationNode>& src) {
  source = &src;
  finished = src[0].terminate;
  waiting = false;
  changeCurrentNode(0);
  response_history.reserve(200);
  history.reserve(200);
}

vector<string>* Conversation::getTypeBuffer() {
  print_buffer.resize(0);
  stringChopper( (*source)[current_node].message, print_buffer, line_width, 
		  (*source)[current_node].speaker.size()+3 );
  return &print_buffer;
}

void Conversation::checkHidden() {
  bool match;
  string lower;
  for (char letter : response) {
    lower.push_back(tolower(letter));
  }
  for (uint i = 0; i < (*source)[current_node].hidden_responses.size(); i++) {
    for (auto&& search_vec : (*source)[current_node].hidden_responses[i].search_terms) {
      match = false;
      for (auto&& term : search_vec) {
	if (lower.find(term) != std::string::npos) {
	  match = true;
	  break;
	}
      }
      if (!match)
	break;
    }
    if (match) {
      if ((*source)[current_node].hidden_responses[i].effect)
	(*source)[current_node].hidden_responses[i].effect_function();
      changeCurrentNode( (*source)[current_node].hidden_responses[i].next_node );
      waiting = !(*source)[current_node].no_response;
      response = "";
      return;
    }
  }
}

void Conversation::addEntryToHistory() {
  history_height++;
  ConversationNode node;  
  node.speaker = "You";
  node.speaker_color = YELLOW_TEXT;
  node.message = response;
  response_history.push_back(node);
  history.push_back(&response_history.back());

}

void Conversation::addResponseToHistory() {
  history_height += menu.items[menu.selected].size();
  response_history.emplace_back();
  ConversationNode& node = response_history.back();
  node.speaker = "You";
  node.speaker_color = YELLOW_TEXT;
  node.message = (*source)[current_node].responses[menu.selected].text;
  history.push_back(&node);
}

void Conversation::enterPressed() { 
  if (history.size() > 198) {
    cerr << "The NPC you were talking to got so bored the game crashed.\n";
    exit(0);
  }
  ConversationNode &node = (*source)[current_node];
  history.push_back( &(node) );
  if ( node.terminate )
    finished = true;
  else if (waiting) {
    if (menu.selected == menu.items.size()) {
      addEntryToHistory();
      if (!node.hidden_responses.empty())
	checkHidden();
      response = "";
    }
    else {
      if (node.responses[menu.selected].effect)
	node.responses[menu.selected].effect_function();
      addResponseToHistory();
      changeCurrentNode( node.responses[menu.selected].next_node );
      waiting = !(*source)[current_node].no_response;
    }
  }
  else if ( node.no_response ) {
    changeCurrentNode( (*source)[current_node].no_response_next );
    waiting = false;
  }  
}

void Conversation::changeCurrentNode(uint id) {
  current_node = id;
  ConversationNode& node = (*source)[current_node];
  if ( !node.no_response ) {
    menu.setNode(node);
    menu.setup(line_width);
    menu_height = menu.height;
  }
  else menu_height = 0;
}

void Conversation::typingDone() {
  if ( !(*source)[current_node].no_response ) {
    waiting = true;
//    history.push_back( &((*source)[current_node]) );
//    menu.setNode((*source)[current_node]);
//    menu.setup(line_width);
  }
}
