//#include <regex>
#include "data_store.hpp"
#include <type_traits>

namespace data_store {

  class Processor {
  public:
    Processor() {
      record_separator = '=';
      field_separator = '-';
      element_separator = ',';
      require_all = false;
      parse_all();
    };
    void setRecordSeparator(char c) {record_separator = c;};
    void setFieldSeparator(char c)   {field_separator = c;};
    void setElementSeparator(char c = ',') {element_separator = c;};
    void parse(vector<string>& properties, const string& file_name);
    template<typename T> void parse(T& t, vector<string>& files);
    template<typename T> void resolve(T& t, vector<string>& files);
    template<typename T> void collect(const string& label, T& dest, 
				      typename enable_if<is_scalar<T>::value >::type* dummy=0);
    void collect(const string& label, string& dest);
    template<typename T>
    void collect(const string& label, vector<T>& dest, vector<string>& properties);
    void requireAllFields(bool in){require_all = in;};
    void genEnum(const string& enum_name, const string& type, const string& prefix, vector<string> items);
  private:
#include DS_PROCESSORS
    template<typename T, typename U> void resolve(T& vec, U& u);
    template<typename T> void test(const string& label, T& dest, string& check);
    bool collect_segment(const string& label, string& result);
    bool require_all;
    char record_separator; 
    char field_separator;
    char element_separator;
    string buffer;
    uint current_iterator;
  };

  template<typename T> void Processor::parse(T& t, vector<string>& files) {
    buffer.reserve(512);
    string temp;
    temp.reserve(128);
    bool just_started = false;
    for (string file : files) {
      std::ifstream inf(DS_DATA_DIRECTORY + file);
      if (!inf.is_open()) {
	cout << "Failed opening " << file << "Exiting. \n";
	exit(0);
      }
      while (inf) {
	std::getline(inf, temp);

	if (temp.empty()) {
	  if (just_started == false) {
	    //	    cout << "New record!\n";
	    t.emplace_back();
	    parse(t.back());
	    buffer.resize(0);
	    just_started = true;
	  }
	}
	else {
	  buffer += temp + "\n";
	  just_started = false;
	}
      }
      inf.close();
    }
  }

  template<typename T> void Processor::test(const string& label, T& dest, string& check) {
    string result;
    if (collect_segment(label, result))
      if (result == check)
	dest = current_iterator;
  }

  template<typename T, typename U> void Processor::resolve(T& vec, U& u) {
    cout << "Debug point 0" << "\n";
    for (current_iterator = 0; current_iterator < vec.size(); current_iterator++)
      resolve(u, vec[current_iterator]);
  }

  template<typename T> void Processor::resolve(T& t, vector<string>& files) {
    buffer.reserve(512);
    string temp;
    uint i = 0;
    temp.reserve(128);
    bool just_started = false;
    for (string file : files) {
      std::ifstream inf(DS_DATA_DIRECTORY + file);
      while (inf) {
	std::getline(inf, temp);
	if (temp.empty()) {
	  if (just_started == false) {
	    //	    cout << "New record!\n";
	    resolve(t, t[i]);
	    buffer.resize(0);
	    i++;
	    just_started = true;
	  }
	}
	else {
	  buffer += temp + "\n";
	  just_started = false;
	}
      }
      inf.close();
    }
  }


  template<typename T> 
  void Processor::collect(const string& label, T& dest, 
			  typename enable_if<is_scalar<T>::value >::type* dummy) {
    string result;
    if (collect_segment(label, result))
      dest = static_cast<T>(std::stoi(result));
  }

  // T must be scalar
  template<typename T> 
  void Processor::collect(const string& label, vector<T>& dest, vector<string>& properties) {
    cout << "properties collect called!" << "\n";
    string result;
    if (collect_segment(label, result))
      for (uint i = 0; i < properties.size(); i++)
	if (result.find(properties[i]) != string::npos) {
	  cout << "size before: "<< dest.size() << "\n";
	  dest.push_back(i);
	  cout << "found something: " << i << "\n";
	  cout << "size after: "<< dest.size() << "\n";
	}
  }

}
