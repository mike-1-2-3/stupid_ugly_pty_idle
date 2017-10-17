#include "herald.hpp"

Herald::Herald() : max(100) {
  newest = -1;
  offset = 0;
//  add(dummy_timestamp);
//  add(dummy_timestamp);
  newer_available = false;
  older_available = false;
}

bool Herald::up() {
  if (older_available) {
    newer_available = true;
    offset++;
    if (offset + iterator + 1 == newest)
      older_available = false;
    if (offset + iterator + 1 > max)
      if ((offset + iterator + 1) - max == newest)
	older_available = false;
    return true;
  } 
  return false;
}

bool Herald::down() {
  if (newer_available) {
    older_available = true;
    offset--;
    if (offset == 0)
      newer_available = false;
    return true;
  }
  return false;
}

vector<string>* Herald::getPrintBuffer(int lines, int line_length) {
  if (items.empty())
    return &print_buffer;
  max_lines = lines;
  line_width = line_length;
  iterator = newest - offset;
  current_line = 0;
  print_buffer.resize(0);
  bool end = false;
  while (end == false) {
    if (!addNext()) {
      end = true;
      older_available = true;
    }
    if (iterator < 0) {
      if (items.size() < max) {
	end = true;
	older_available = false;
      }
      else
	iterator = max - 1;
    }
  }

  return &print_buffer;
}

void reverse(vector<string>& strs, int first, int last) {
  int num_swaps = ((last - first) / 2) + 1;
  for (int i = 0; i < num_swaps; i++)
    strs[first+i].swap(strs[last-i]);
}

bool Herald::addNext() {

  string tmp = items[iterator].timestamp + ' ' + *(items[iterator].text);
  int start_char = 0;
  int length;
  int start_line = current_line;
  bool end = false;
  bool first = true;
  bool remaining_space = true;

  while (!end && remaining_space) {
    length = line_width;
    if (start_char + line_width >= tmp.size()) {
      length = tmp.size() - start_char;
      end = true;
    }
    else while(length > 0 && tmp[start_char + length] != ' ')
	length--;
    if (first) { 
      first = false;
      print_buffer.push_back(tmp.substr(start_char, length));
    }
    else 
      print_buffer.push_back(' ' + tmp.substr(start_char, length));
    start_char += length;
    current_line++;
    if (current_line >= max_lines)
      remaining_space = false;
  }

  reverse(print_buffer, start_line, current_line - 1);
  if (remaining_space) {
    print_buffer.push_back(" ");
    current_line++;
  }
  iterator--;

  return remaining_space;
}

void Herald::add(string& timestamp, string& message, color_pair color) {

  if (items.size() < max) {
    newest++;
    items.emplace_back(message, timestamp, color);
  }
  else {
    newest++;
    if (newest == max)
      newest = 0;
    items[newest].text = &message;
    items[newest].timestamp = timestamp;
    items[newest].color = color;
  }
}
