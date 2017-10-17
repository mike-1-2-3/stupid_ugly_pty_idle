
#include "data_store_base.hpp"

using namespace data_store;

DataStoreBase::DataStoreBase() {
  for (uint i = 0; i < Snum_streams; i++)
    streams.emplace_back();
  segment_size = 0;
}

DataStoreBase::~DataStoreBase() {
  for (auto& item : streams)
    if (item.is_open()) {
      item.flush();
      item.close();
    }
}

bool DataStoreBase::open(string file_name, std::ios_base::openmode mode, Stream selector) {
  current = &streams[selector];
  if (current->is_open()) {
    current->flush();
    current->close();
  }
  current->open(file_name, mode);
  return current->is_open();
}

void DataStoreBase::close(Stream selector) {
  streams[selector].close();
}

size_t DataStoreBase::serialize_item(string& str) {
  size_t size = str.size();    
  current->write((char*)&size, sizeof(size));
  current->write((char*)&str[0], size*sizeof(char) );
  size += sizeof(size_t);
  return size;
}

void DataStoreBase::deserialize_item(string& str) {
  size_t size;
  current->read((char*)&size, sizeof(size_t));
  str.resize(size);
  current->read((char*)&str[0], size*sizeof(char));
}

size_t DataStoreBase::serialize_item(vector<string>& str) {
  size_t size = str.size();    
  current->write((char*)&size, sizeof(size));
  for (uint i = 0; i < str.size(); i++)
    serialize_item(str[i]);
  size += sizeof(size_t);
  return size;
}

void DataStoreBase::deserialize_item(vector<string>& str) {
  size_t size;
  current->read((char*)&size, sizeof(size_t));
  str.resize(size);
  for (uint i = 0; i < str.size(); i++)
    deserialize_item(str[i]);
}

