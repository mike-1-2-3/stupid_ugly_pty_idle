
#ifndef SIM_GAME_CONVERSATION_GENERATOR
#define SIM_GAME_CONVERSATION_GENERATOR

//#include "conversation.hpp"
//#include "person.hpp"
#include "common.hpp"
#include "enums/court_events.hpp"
#include "enums/colors.hpp"
#include "conversation_node.hpp"

class Person;
class GameEngine;

class ConversationGenerator {
public:
  ConversationGenerator(GameEngine& owner) : ge(owner) {};
  GameEngine& ge;
  void generateCharGen(Person& caer);
  void generateGethinRescue(Person& gethin);
  void generateCourtEvent(Person& usher, CourtEvent event);
private:
  void generateNewPeasantEvent(Person& usher, CourtEvent event);
  ConversationNode* newNode(vector<ConversationNode>& convo, string speaker,
			    color_pair color);
  ResponsePair* newResponse(vector<ConversationNode>& convo);
};

#endif
