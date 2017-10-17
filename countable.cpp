
#include "countable.hpp"

uint isFood(uint t) {
  if (t >= CI_Brown_Trout && t <= CI_Sunroot)
    return t - CI_Brown_Trout;
  return 0;
}
