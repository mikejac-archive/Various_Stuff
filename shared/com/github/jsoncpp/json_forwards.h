#ifndef JSON_FORWARDS_H_INCLUDED
# define JSON_FORWARDS_H_INCLUDED

#include "json_config.h"
#include <stdint.h>

namespace json {

   // writer.h
   class FastWriter;
   class StyledWriter;

   // reader.h
   class Reader;

   // features.h
   class Features;

   // value.h
   typedef int Int;
   typedef unsigned int UInt;
   typedef int64_t Int64;
   typedef uint64_t UInt64;
   class StaticString;
   class Path;
   class PathArgument;
   class Value;
   class ValueIteratorBase;
   class ValueIterator;
   class ValueConstIterator;
#ifdef JSON_VALUE_USE_INTERNAL_MAP
   class ValueAllocator;
   class ValueMapAllocator;
   class ValueInternalLink;
   class ValueInternalArray;
   class ValueInternalMap;
#endif // #ifdef JSON_VALUE_USE_INTERNAL_MAP

} //namespace json {


#endif // JSON_FORWARDS_H_INCLUDED
