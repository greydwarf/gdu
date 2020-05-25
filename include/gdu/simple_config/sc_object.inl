#ifndef SC_OBJECT_H_7GAVULTZ
#define SC_OBJECT_H_7GAVULTZ
#include "gdu/simple_config/sc_value.h"

inline void gdu::SCObject::add(const std::string& key, const SCValue& value) {
   values_.emplace(key, value);
}

inline const gdu::SCValue& gdu::SCObject::operator[](const std::string& key) const {
   static const SCValue NIL;
   auto x = values_.find(key);
   if (x == values_.end()) return NIL;
   return x->second;
}

inline size_t gdu::SCObject::size() const {
   return values_.size();
}

#endif /* end of include guard: SC_OBJECT_H_7GAVULTZ */
