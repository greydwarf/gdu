#ifndef SC_OBJECT_H_5AVALFOS
#define SC_OBJECT_H_5AVALFOS

#include <iterator>
#include "gdu/flat_map.h"

namespace gdu {
   class SCValue;
   class SCObject {
      using map_type = gdu::flat_map<std::string, SCValue>;
   public:
      using value_type=map_type::value_type;
      using reference=value_type&;
      using const_reference=const value_type&;
      using const_iterator=typename map_type::const_iterator;
      using difference_type=typename std::iterator_traits<const_iterator>::difference_type;
      using size_type=typename map_type::size_type;

      SCObject() = default;
      void add(const std::string& key, const SCValue& reference);
      [[nodiscard]] const SCValue& operator[](const std::string& key) const;
      [[nodiscard]] const_iterator begin() const;
      [[nodiscard]] const_iterator end() const;
      [[nodiscard]] size_t size() const;
   private:
      flat_map<std::string, SCValue> values_;
   };
}
#endif /* end of include guard: SC_OBJECT_H_5AVALFOS */

#include "gdu/simple_config/sc_object.inl"
