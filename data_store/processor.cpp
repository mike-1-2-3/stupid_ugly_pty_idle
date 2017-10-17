
#include "processor.hpp"

using namespace data_store;

void Processor::genEnum(const string& enum_name, const string& type, const string& prefix, 
			vector<string> items) {
  std::ofstream of("enums/" + enum_name + ".hpp", fstream::out);
  of << "// This file was automatically generated, it'll get clobbered when you run process.\n\n";
  of << "#ifndef AUTO_ENUM_" << enum_name << "\n#define AUTO_ENUM_" << enum_name << "\n\n";
  of << "enum " << enum_name << " : " << type << " {\n";
  if (items.size() == 0) {
    cout << "empty item list in enum gen. exiting\n";
    exit(0);
  }
  for (auto& item : items)
    for (auto& ch : item) 
      if (ch == ' ')
	ch = '_';
  of << prefix + items[0];
  for (uint i = 1; i < items.size(); i++)
    of << ", " << prefix << items[i];
  of << "\n};\n\n#endif";
  of.close();
}

bool Processor::collect_segment(const string& label, string& result) {
  size_t found_at, start, end /*, temp*/;
  found_at = buffer.find(label);
  if (found_at == string::npos) {
    if (require_all)
      cout << "Parsing error - required field not found: " << label <<"\n";
    else return false;
  }
  start = found_at + label.size();
//  end = buffer.find_first_of(' ', start);
//  temp = buffer.find_first_of('\n', start);
//  if (temp < end)
//    end = temp;
//  cout << "end is: " << end << "\n";
  end = buffer.find_first_of('\n', start);
  if (end == string::npos)
    cout << "Parsing error: no terminator found after " << label << "\n";
  result = buffer.substr(start, end - start);
  return true;
}

void Processor::parse(vector<string>& properties, const string& file_name) {
  std::ifstream inf(DS_DATA_DIRECTORY + file_name);
  string temp;
  while (inf) {
    std::getline(inf, temp);
    if (!temp.empty()) {
      properties.push_back(temp);
    }
  }  
}

void Processor::collect(const string& label, string& dest) {
  collect_segment(label, dest);
}

int main() {
  Processor ps;
}
