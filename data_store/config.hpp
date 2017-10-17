#ifndef DS_CONFIG
#define DS_CONFIG

#include "ds_common.hpp"

#include "../game_engine.hpp"

#define DS_SERIALIZERS "serializers.hpp"
#define DS_PRE_SERIALIZERS "processor_serializers.hpp"
#define DS_PROCESSORS "processors.hpp"
#define DS_DATA_DIRECTORY "data/"
#define LANG "en"

// These name the file streams you can split data members between.
namespace data_store {
  enum Stream : uchar {Smaster, Ssave, Snum_streams};
}
#define DEFAULT_STREAM Smaster



#endif
