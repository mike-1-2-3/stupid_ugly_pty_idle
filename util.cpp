
#include "util.hpp"

uint randRange(uint min, uint max) {
  return min + (rand() % (max - min));
}

void stringChopper(string& long_string, vector<string>& segments, uint line_width, uint prefix_chars) {
  uint start_char = 0;
  bool end = false;
  bool first = true;
  uint length;

  while(!end) {
    length = line_width;
    if (first) {
      length -= prefix_chars;
      first = false;
    }
    if (start_char + length >= long_string.size()) {
      length = long_string.size() - start_char;
      end = true;
    }
    else while(length > 0 && long_string[start_char + length] != ' ')
	   length--;
    segments.push_back(long_string.substr(start_char, length));
    start_char += length;
  }
}

