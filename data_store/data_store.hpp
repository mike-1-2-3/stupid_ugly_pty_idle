
// This code is stupid and incomplete. Don't expect it to work.

#ifndef DS_DATA_STORE
#define DS_DATA_STORE

#include "data_store_base.hpp"

namespace data_store {

  class DataStore : public DataStoreBase {
  public:

    DataStore(){};  
    template<typename T, typename enable_if<is_enum<T>::value >::type* = nullptr, typename... Args> 
    void serialize(T n = DEFAULT_STREAM, Args&... args) {
      current = &streams[n];
      serialize(args...);
    };
    template<typename T, typename enable_if<is_enum<T>::value >::type* = nullptr, typename... Args> 
    void deserialize(T n = DEFAULT_STREAM, Args&... args) {
      current = &streams[n];
      deserialize(args...);
    };
    template<typename... Args> void serialize(Args&... args);
    template<typename... Args> void deserialize(Args&... args);

#ifdef DS_PROC_BUILD
  #include DS_PRE_SERIALIZERS
#else
  #include DS_SERIALIZERS
#endif

  private:
    template<typename T> void serialize_internal(T& item);
    template<typename T, typename... Args> void serialize_internal(T& first, Args&... args);
    template<typename T> void deserialize_internal(T& item);
    template<typename T, typename... Args> void deserialize_internal(T& first, Args&... args);

    // Bring separately compiled string serializers into scope.
    using DataStoreBase::serialize_item;
    using DataStoreBase::deserialize_item;

    // ints/chars, floats, enums
    template <class T> inline
    size_t serialize_item(T& t, typename enable_if<is_scalar<T>::value >::type* dummy = 0);
    template <class T> inline
    void deserialize_item(T& t, typename enable_if<is_scalar<T>::value >::type* dummy = 0);

    // Single class
    template <class T> inline
    size_t serialize_item(T& t, typename enable_if<is_class<T>::value >::type* dummy = 0);
    template <class T> inline
    void deserialize_item(T& t, typename enable_if<is_class<T>::value >::type* dummy = 0);

    // Vectors of ints/chars, floats, or enums
    template<typename T>
    size_t serialize_item(vector<T>& vec, typename enable_if<is_scalar<T>::value >::type* dummy = 0);
    template<typename T>
    void deserialize_item(vector<T>& vec, typename enable_if<is_scalar<T>::value >::type* dummy = 0 );

    // Vectors of classes
    template<typename T>
    size_t serialize_item(vector<T>& vec, typename enable_if<is_class<T>::value >::type* dummy = 0 );
    template<typename T>
    void deserialize_item(vector<T>& vec, typename enable_if<is_class<T>::value >::type* dummy = 0 );
  };



  // Variadic serialize base case
  template<typename T>
  void DataStore::serialize_internal(T& item) {
    segment_size += serialize_item(item);
  }
  template<typename T, typename... Args>
  void DataStore::serialize_internal(T& first, Args&... args) {
    serialize_internal(first);
    serialize_internal(args...);
  }
  // Wrapper to allow setting the file
  template<typename... Args>
  void DataStore::serialize(Args&... args) {
    serialize_internal(args...);
  }

  // Variadic deserialize base case
  template<typename T>
  void DataStore::deserialize_internal(T& item) {
    deserialize_item(item);
  }
  template<typename T, typename... Args>
  void DataStore::deserialize_internal(T& first, Args&... args) {
    deserialize_internal(first);
    deserialize_internal(args...);
  }
  // Wrapper to allow setting the file
  template<typename... Args>
  void DataStore::deserialize(Args&... args) {
    deserialize_internal(args...);
  }

  // ints/chars, floats, enums
  template <class T> inline
  size_t DataStore::serialize_item(T& t, typename enable_if<is_scalar<T>::value >::type* dummy) {
    cout << " serializing: " << t << " at " << current->tellp() <<"\n";
    current->write((char*)&t, sizeof(t));
    return sizeof(t);
  }
  template <class T> inline
  void DataStore::deserialize_item(T& t, typename enable_if<is_scalar<T>::value >::type* dummy) {
    current->read((char*)&t, sizeof(t));
    cout << " deserializing: " << t << " at " << current->tellg() <<"\n";
  }

  // Single object
  template <class T> inline
  size_t DataStore::serialize_item(T& t, typename enable_if<is_class<T>::value >::type* dummy) {
    save(t);
    return 1;
  }
  template <class T> inline
  void DataStore::deserialize_item(T& t, typename enable_if<is_class<T>::value >::type* dummy) {
    load(t);
  }

  // Vectors of ints/chars, floats, or enums
  template<typename T>
  size_t DataStore::serialize_item(vector<T>& vec, 
				   typename enable_if<is_scalar<T>::value >::type* dummy) {
    size_t size = vec.size();    
//    current->write((char*)&size, sizeof(size));
    serialize_item(size);
    current->write((char*)&vec[0], size*sizeof(T) );
    size += sizeof(size_t);
    return size;
  }
  template<typename T>
  void DataStore::deserialize_item(vector<T>& vec, 
				   typename enable_if<is_scalar<T>::value >::type* dummy) {
    size_t size;
    current->read((char*)&size, sizeof(size_t));
    vec.resize(size);
    current->read((char*)&vec[0], size * sizeof(T));
  }

  // Vectors of classes
  template<typename T>
  size_t DataStore::serialize_item(vector<T>& vec, 
				   typename enable_if<is_class<T>::value >::type* dummy) {
    size_t size = vec.size();    
    current->write((char*)&size, sizeof(size));
    for(T item : vec) {
//      cout << "starting to serialize" << item.name<<"\n";
      save(item);
    }
    size += sizeof(size_t);
    return size;
  }
  template<typename T>
  void DataStore::deserialize_item(vector<T>& vec, 
				   typename enable_if<is_class<T>::value >::type* dummy) {
    size_t size;
    current->read((char*)&size, sizeof(size_t));
    vec.reserve(size);
    cout << "The size of deserialized vector of classes is: " << size << "\n";
    for(uint i = 0; i < size; i++) {
      vec.emplace_back();
      load(vec[i]);
    }
  }

}

#endif
