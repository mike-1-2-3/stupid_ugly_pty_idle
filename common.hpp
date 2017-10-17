

#ifndef SIM_GAME_COMMON
#define SIM_GAME_COMMON

#include <vector>
#include <iostream>

using std::vector;
using std::cout;
using std::cerr;
using std::string;
using std::to_string;

typedef unsigned int uint;
typedef unsigned char uchar;
typedef unsigned long ulong;

struct Effable {
  string name;
  string description;
  vector<string> properties;
};

struct Stack {
  uint type;
  uint amount;
};

struct Triple {
  uint x,y,z;
};

#endif

