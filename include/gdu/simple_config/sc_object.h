#ifndef SC_OBJECT_H_5AVALFOS
#define SC_OBJECT_H_5AVALFOS

#include <iterator>
#include "gdu/flat_map.h"
#include "gdu/simple_config/sc_value.h"

namespace gdu {
   class SCObject {
   public:
      using key_type = std::string;
      using mapped_type=SCValue;
      using value_type=std::pair<key_type,mapped_type>;
      using reference=value_type&;
      using const_reference=const value_type&;
      using iterator=typename std::vector<value_type>::iterator;
      using reverse_iterator=typename std::vector<value_type>::reverse_iterator;
      using const_iterator=typename std::vector<value_type>::const_iterator;
      using const_reverse_iterator=typename std::reverse_iterator<const_iterator>;
      using difference_type=typename std::iterator_traits<const_iterator>::difference_type;
      using size_type=typename std::vector<value_type>::size_type;

      SCObject() = default;
      void add(const std::string& key, const SCValue& reference);
      const SCValue& operator[](const std::string& key) const;
      const_iterator begin() const;
      const_iterator end() const;
   private:
      flat_map<std::string, SCValue> values_;
   };
}

void gdu::SCObject::add(const std::string& key, const SCValue& value) {
   values_.emplace(key, value);
}

const gdu::SCValue& gdu::SCObject::operator[](const std::string& key) const {
   static const SCValue NIL;
   auto x = values_.find(key);
   if (x == values_.end()) return NIL;
   return x->second;
}

#endif /* end of include guard: SC_OBJECT_H_5AVALFOS */
