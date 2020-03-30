#ifndef SIZE_H_07CZ9KQI
#define SIZE_H_07CZ9KQI
#include <type_traits>
namespace gdu {
   //! This is a function from C++20 that I love, and it is simple enough to implement.
   //! No calls to raw size() anymore!
   static_assert(sizeof(ssize_t) >= 4, "The GDU library requires size_t be at least 4 bytes");
   template <typename T>
   [[nodiscard]] constexpr ssize_t ssize(const T& c) noexcept 
   { return static_cast<ssize_t>(c.size()); }
}
#endif
