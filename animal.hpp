
#ifndef DS_ANIMAL
#define DS_ANIMAL

#include "common.hpp"

struct Animal {
  Animal(){};
  string name;
  int weight;
  int max_age;
  int prey;
  vector<uchar> properties;
//  vector<int> vec;
};

#endif
