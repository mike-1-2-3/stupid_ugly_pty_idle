#include "config.hpp"
#include <fstream>

namespace data_store {

  using std::fstream;
  using std::ios;
  using std::enable_if;
  using std::is_class;
  using std::is_scalar;
  using std::is_enum;

  class DataStoreBase {
  public:
    bool open(string file_name, std::ios_base::openmode mode, Stream selector = DEFAULT_STREAM);
    void close(Stream selector = DEFAULT_STREAM);
    ~DataStoreBase();
  protected:
    DataStoreBase();
    size_t serialize_item(string& str);
    void deserialize_item(string& str);
    size_t serialize_item(vector<string>& str);
    void deserialize_item(vector<string>& str);
    void setInStream(uchar n);
    void setOutStream(uchar n);
    size_t segment_size;
    vector<fstream> streams;
    fstream* current;
  };
}
