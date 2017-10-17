#include "conversation_generator.hpp"
#include "game_engine.hpp"
#include "person.hpp"
#include "enums/menus.hpp"
#include "enums/misc.hpp"

ConversationNode* ConversationGenerator::newNode(vector<ConversationNode>& convo, string speaker, 
						 color_pair color) {
  ConversationNode node;
  node.speaker = speaker;
  node.speaker_color = color;
  node.generic_entries = false;
  node.no_response = true;
  node.terminate = false;
  node.no_response_next = convo.size()+1;
  convo.push_back(node);
  return &(convo.back());
}

ResponsePair* ConversationGenerator::newResponse(vector<ConversationNode>& convo) {
  convo.back().no_response = false;
  convo.back().responses.emplace_back();
  convo.back().responses.back().next_node = convo.size();
  convo.back().responses.back().effect = false;
  return &convo.back().responses.back();
}

void ConversationGenerator::generateCourtEvent(Person& usher, CourtEvent event) {
  switch (event) {
    case CE_land_request :
      generateNewPeasantEvent(usher, event);
      break;
  }
}
void ConversationGenerator::generateNewPeasantEvent(Person& usher, CourtEvent event) {
  EntryPair entry;
  uint ni = rand() % ge.male_names_western.size();
  string name = ge.male_names_western[ni];
  ConversationNode* node;
  ResponsePair* response;

  node = newNode(usher.convos, usher.eff.name, WHITE_TEXT);
  node->message = "M'lord, a local peasant has come of age.";
  
  Triple coord = ge.player.getRandomVirgate();
  cerr << "coor x y z:" << coord.x << " " << coord.y << " " <<coord.z << "\n";
  Virgate& candidate = ge.world.getVirgate(coord);
  string m, place;
  place = "the " + candidate.eff.name + " of " + 
    ge.world.provinces[ge.player.current_province].eff.name;
  uint r = rand() % 3;
  if (r == 0)
    m = "Yes sire, I'd like to settle the land at " + place + ". My brothers and "
      "sisters can help my parents at home, I'd find greater fortune out on my own.";
  else if (r == 1)
    m = "Indeed m'lord, my family's land can't support us all. Your grace willing I'd move to "
      + place + ".";
  else 
    m = "Aye m'lord, I been kicked out ter the curb I 'ave! Time ter spread my wings an' all that. I heard talk o' good prospects o'er at "
      + place + ".";

  node = newNode(usher.convos, name, BRIGHT_GREY_TEXT);
  node->message = m;

  response = newResponse(usher.convos);
  response->text = "Very well. This land shall be yours and your descendents as long as it is properly"
    " utilized and thy taxes are paid. " + usher.eff.name + " will take you to the steward to "
    "formalize the contract.";
  response->effect = true;
  response->effect_function = [&, name, coord](void){
    candidate.eff.name = name + "'s " + ge.settled_land_names[candidate.type];
    ge.player.addVirgate(coord);
  };
  response = newResponse(usher.convos);
  response->text = "You can work the land, boy, but remember, if a man in my lands neglects his duties "
    "the penalties are harsh. Go.";
  response = newResponse(usher.convos);
  response->text = "I have need of stout men to fight for the safety and glory of Maredelus.";
  
  node = newNode(usher.convos, name, BRIGHT_GREY_TEXT);
  node->message = "Gratitude, my liege!";
  node->terminate = true;

}

void ConversationGenerator::generateGethinRescue(Person& gethin) {
  uint current_node = 0;
  ConversationNode node;
  ResponsePair response;
  EntryPair entry;

  // 0
  node.speaker = "Gethin";
  node.speaker_color = WHITE_TEXT;
  node.message = "That was a close call! Thanks, m'lord!";
  node.generic_entries = false;
  node.no_response = true;
  node.terminate = true;
  current_node++;
  node.no_response_next = current_node;
  gethin.convos.push_back(node);

}

void ConversationGenerator::generateCharGen(Person& caer) {
  uint current_node = 0;
  ConversationNode node;
  ResponsePair response;
  EntryPair entry;

  // 0
  node.speaker = "Narrator";
  node.speaker_color = WHITE_TEXT;
  node.message = "All you can see is a collage of swirling purples and greys, they make you think of bruises and stone. You feel doomed. And then a figure begins to take shape. A beautiful woman, who speaks in a smooth voice that lifts the fear. <Press Enter>";
  node.generic_entries = false;
  node.no_response = true;
  node.terminate = true;
  current_node++;
  node.no_response_next = current_node;
  caer.convos.push_back(node);

  // 1
  node.speaker = caer.eff.name;
  node.speaker_color = PINK_TEXT;
  node.message = "Silly mortal, wasting time in a dream when the world is so big and beautiful. "
    "Well, I'll try to make the dream a good one. If you "
    "insist on staying, you should be aware of some things.";
  node.generic_entries = false;
  node.no_response = true;
  node.terminate = false;
  current_node++;
  node.no_response_next = current_node;
  caer.convos.push_back(node);

  // 2
  node.speaker = caer.eff.name;
  node.speaker_color = PINK_TEXT;
  node.message = "First of all, nothing can stop the passage of time. When you return to the dream, you "
    "should see if the royal herald has reported anything important.";
  node.generic_entries = false;
  node.no_response = true;
  node.terminate = false;
  current_node++;
  node.no_response_next = current_node;
  caer.convos.push_back(node);

//---------------------- 3
  node.speaker = caer.eff.name;
  node.speaker_color = PINK_TEXT;
  node.message = "Second, you must be clever. Don't act without knowing the details of your situation, "
    "and don't speak without thinking first. For example, tell me who I am.";
  node.generic_entries = false;
  node.no_response = false;
  node.terminate = false;
    response.text = "You're the.... ermm... Princess of Naktesh.";
    response.next_node = 6;
    node.responses.push_back(response);
    response.text = "You're the Grand Poobah of Winselton, of course!";
    response.next_node = 6;
    node.responses.push_back(response);
    entry.effect = false;
    entry.next_node = 4;
    entry.search_terms.resize(2);
    entry.search_terms[0] = {"goddess", "deity", "fairy", "fey ", "deities", "lady", "lover"};
    entry.search_terms[1] = {"dream", "sleep", "swan", "crane", "unconscious", "somnolence", "slumber",
                             "youth", "love", "angis", "aengis"};
    node.hidden_responses.push_back(entry);
    entry.next_node = 10;
    entry.search_terms.resize(1);
    entry.search_terms[0] = {"caer ibormeith", " hot ", "sexy"};
    node.hidden_responses.push_back(entry);
  current_node++;
  caer.convos.push_back(node);
  node.responses.resize(0);
  node.hidden_responses.resize(0);
//----------------------- 

  //4
  node.speaker = caer.eff.name;
  node.speaker_color = PINK_TEXT;
  node.message = "Indeed. If you get stuck, remember that you might need to solve a riddle and say something clever. Now then. Unfortunately, it seems you're unconscious because you took a whack to the head. I'll help you recover the memories that were destroyed. ";
  node.generic_entries = false;
  node.no_response = true;
  node.terminate = false;
  current_node++;
  node.no_response_next = 7;
  caer.convos.push_back(node);

  //5
  node.speaker = caer.eff.name;
  node.speaker_color = PINK_TEXT;
  node.message = "Very good. I think your amnesia is wearing off now. I expect great things from you, mortal. I will visit you again. Now wake up, looks like you're in some trouble...";
  node.generic_entries = false;
  node.no_response = true;
  node.terminate = false;
  current_node++;
  node.no_response_next = 11;
  caer.convos.push_back(node);

  //6
  node.speaker = caer.eff.name;
  node.speaker_color = PINK_TEXT;
  node.message = "Try again. I'm looking for two key words.";
    response.text = "You're the.... ermm... princess of Naktesh.";
    response.next_node = 6;
    node.responses.push_back(response);
    response.text = "You're the Grand Poobah of Winselton, of course!";
    response.next_node = 6;
    node.responses.push_back(response);
    entry.effect = false;
    entry.next_node = 4;
    entry.search_terms.resize(2);
    entry.search_terms[0] = {"goddess", "deity", "fairy", "fey", "deities", "lady", "lover"};
    entry.search_terms[1] = {"dream", "sleep", "swan", "crane", "unconscious", "somnolence", 
			     "love", "youth", "slumber", "aengis", "angis"};
    node.hidden_responses.push_back(entry);
    entry.next_node = 10;
    entry.search_terms.resize(1);
    entry.search_terms[0] = {"caer ibormeith", " hot", "sexy"};
    node.hidden_responses.push_back(entry);
  node.generic_entries = false;
  node.no_response = false;
  node.terminate = false;
  current_node++;
  node.no_response_next = 2;
  caer.convos.push_back(node);
  node.responses.resize(0);
  node.hidden_responses.resize(0);

  //7
  node.speaker = caer.eff.name;
  node.speaker_color = PINK_TEXT;
  node.message = "Let's start with your name.";
  node.generic_entries = false;
  node.no_response = false;
  node.terminate = false;
    entry.effect = true;
    entry.next_node = 8;
    entry.search_terms.resize(1);
    entry.search_terms[0] = {""};
    entry.effect_function = [&](void)
      {ge.people[0].eff.name = ge.converser.response;
       ge.display.drawConvoTitle(ge.headings[HI_conversation], ge.people[0].eff.name, caer.eff.name);};
    node.hidden_responses.push_back(entry);
  current_node++;
  caer.convos.push_back(node);
  node.responses.resize(0);
  node.hidden_responses.resize(0);

  //8
  node.speaker = caer.eff.name;
  node.speaker_color = PINK_TEXT;
  node.message = "You are the only child of Eochaid, lord of the small town of Bailedoire, capital of Maredelus. When you were just six years old, your father sent you far to the south, past the province of Nathwyn to the city of Alethwayn, the greatest city in all of Sidhe, to study at the temple of Halieth. What was the focus of your studies?";
    response.text = "I studied agronomy and botony under herbologist Setritheir.";
    response.next_node = 9;
    node.responses.push_back(response);
    response.text = "I read the chronicles and manuscripts of all the great rulers of the past, learning their philosophies and tactics as directed by the head archivist Oberidain.";
    response.next_node = 9;
    node.responses.push_back(response);
    response.text = "I learned to see into the distant planes and study the divine energies with head druid Cathbad.";
    response.next_node = 9;
    node.responses.push_back(response);
    response.text = "I learned to touch the scintillating web of power betwixt Sidhe and Annwyn under archmage Gallatos.";
    response.next_node = 9;
    node.responses.push_back(response);
  node.generic_entries = false;
  node.no_response = false;
  node.terminate = false;
  current_node++;
  node.no_response_next = 2;
  caer.convos.push_back(node);
  node.responses.resize(0);
  node.hidden_responses.resize(0);

//9
  node.speaker = caer.eff.name;
  node.speaker_color = PINK_TEXT;
  node.message = "A worthy pursuit. Fret not, your race is long lived, and there exist means to live indefinitely, even for mortals. You may have time to learn any study you wish. And in your free time, what did you do?";
    response.text = "Trained relentlessly with sword and shield.";
    response.next_node = 5;
    node.responses.push_back(response);
    response.text = "Walked the shadowy groves in search of sprites and beasts to study.";
    response.next_node = 5;
    node.responses.push_back(response);
    response.text = "Explored the caves and mountains.";
    response.next_node = 5;
    node.responses.push_back(response);
    response.text = "I loitered in the markets, looking at new goods and occasionally buying goods to resell at a higher price.";
    response.next_node = 5;
    node.responses.push_back(response);
  node.generic_entries = false;
  node.no_response = false;
  node.terminate = false;
  current_node++;
  node.no_response_next = 2;
  caer.convos.push_back(node);
  node.responses.resize(0);
  node.hidden_responses.resize(0);

// 10
  node.speaker = caer.eff.name;
  node.speaker_color = PINK_TEXT;
  node.message = "Are you trying to be cute, little churl? Watch your tongue around the gods. There are ways to die permanently. You'll be deleted beyond recovery, if you catch my meaning.";
  node.generic_entries = false;
  node.no_response = true;
  node.terminate = false;
  current_node++;
  node.no_response_next = 6;
  caer.convos.push_back(node);

// 11
  node.speaker = "Narrator";
  node.speaker_color = WHITE_TEXT;
  node.message = "You awaken, reinvigorated by the magic of the goddess. It's a warm day, and you remember "
    "that you're on your way back home after spending years in Alethwayn. You're in the Roekil woods, just "
    "south of your territory. You were accosted by bandits and knocked from your horse. In a sudden panic "
    "you look around and see your servant Gethin frantically dodging blows and rush to his aid.";
  node.generic_entries = false;
  node.no_response = true;
  node.terminate = false;
  current_node++;
  node.no_response_next = 12;
  caer.convos.push_back(node);

// 12
  node.speaker = "Narrator";
  node.speaker_color = WHITE_TEXT;
  node.message = "[All you need to do during combat is watch, you can ask people how it works later]";
  node.generic_entries = false;
  node.no_response = true;
  node.terminate = true;
  current_node++;
  node.no_response_next = 12;
  caer.convos.push_back(node);

}
