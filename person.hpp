
#ifndef SIM_GAME_PERSON
#define SIM_GAME_PERSON

#include "common.hpp"
#include "conversation.hpp"
#include "enums/skills.hpp"

struct Person {
  Effable eff;
  uchar species;
  uchar loyalty;
  uchar brain;
  uchar magic;
  uchar size;
  uchar speed;
  void pickName(vector<string>& names);
  void addExp(SkillIndex skill, uint amount);
  uint expToNext(uint skill);
  vector<uint> skills;
  vector<ulong> exp_progress;
  vector<ConversationNode> convos;
};

#endif
