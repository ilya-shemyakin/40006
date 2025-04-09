#include "DataStruct.h"

std::ostream &operator<<(std::ostream &ostream, DataStruct struc) {
  ostream << "(:key1 " << struc.key1 << ":key2 " << abs(struc.key2) << ":key3 "
          << struc.key3 << ":)";

  return ostream;
}