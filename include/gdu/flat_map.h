#include <vector>
#include <algorithm>
#include <functional>
#include <memory>

namespace gdu {
//! gdu::flat_map is an exercise I wrote to learn about implementing containers in C++14.
//! It hasn't been tested operationally.  If you really want to use something like this, I suggest
//! using boost's flat_map instead.  That has been tested, and likely has far fewer bugs than this
//! implementation, which will likely cause you no end of grief.  Really, don't use this.
//!
//! A flat_map is an associative container that is optimized for only a few entries (less than 30).
//! It supports unique keys (contains at most one of each key value) and provides for fast retrieval 
//! of values of another type T based on the keys.
//!
//! A flat_map satisfies most of the requirements of a container, a reversible
//! container and an associative container. A flat_map also provides most operations described 
//! for unique keys. For a flat_map<Key,T> the key_type is Key and the value_type is 
//! std::pair<Key,T> (unlike std::map<Key, T> which value_type is std::pair<<b>const</b> Key, T>).
//!
//! flat_map is similar to std::map but it's implemented using a vector instead of a binary tree.
//!
//! Using a sequential sequence container means that inserting a new element into a flat_map invalidates
//! previous iterators and references.  Similarly, erasing an invalidates iterators and 
//! references pointing to elements that come after the erased element.
//!
//! This container provides random-access iterators.
//!
//! \tparam Key is the key_type of the map
//! \tparam Value is the <code>mapped_type</code>
//! \tparam Compare is the ordering function for Keys (e.g. <i>std::less<Key></i>).
//! \tparam AllocatorOrContainer is the allocator to allocate <code>value_type</code>s (e.g. <i>allocator< std::pair<Key, T> > </i>).
template <typename K, typename V, 
         class Less = std::less<K>,
         class Alloc = std::allocator<std::pair<K,V>>> 
class flat_map {
public:
   using key_type = K;
   using mapped_type=V;
   using value_type=std::pair<K,V>;
   using key_compare=Less;
   using allocator_type=Alloc;
   using reference=value_type&;
   using const_reference=const value_type&;
   using pointer=typename std::allocator_traits<allocator_type>::pointer;
   using const_pointer=typename std::allocator_traits<allocator_type>::const_pointer;
   using iterator=typename std::vector<value_type>::iterator;
   using reverse_iterator=typename std::vector<value_type>::reverse_iterator;
   using const_iterator=typename std::vector<value_type>::const_iterator;
   using const_reverse_iterator=typename std::reverse_iterator<const_iterator>;
   using difference_type=typename std::iterator_traits<const_iterator>::difference_type;
   using size_type=typename std::vector<value_type>::size_type;

   //! <b>Effects</b>: Default constructs an empty flat_map.
   //!
   //! <b>Complexity</b>: Constant.
   flat_map() = default;

   //! <b>Effects</b>: Constructs an empty flat_map and inserts elements from the 
   //! initailizer_list of key/value pairs. This function
   //! is more efficient than default-creating the flat_map and then inserting 
   //! each entry separately.
   //!
   explicit flat_map(std::initializer_list<value_type> list) : v_(list) {
      std::sort(std::begin(v_), std::end(v_), key_comp);
      auto it = std::unique(v_.begin(), v_.end(), 
                            [](const value_type& a, const value_type& b) { 
                              return a.first == b.first; 
                            });
      v_.resize(std::distance(v_.begin(), it));
   }

   explicit flat_map(std::vector<value_type>& list) : v_(list) {
      std::sort(std::begin(v_), std::end(v_), key_comp);
      auto it = std::unique(v_.begin(), v_.end(), 
                            [](const value_type& a, const value_type& b) { 
                              return a.first == b.first; 
                            });
      v_.resize(std::distance(v_.begin(), it));
   }

   flat_map(const flat_map<K,V,Less,Alloc>& other) = default;
   ~flat_map() = default;

   flat_map(flat_map<K,V, Less,Alloc>&& other) noexcept
      : v_(std::move(other.v_))
           {}

   flat_map& operator=(const flat_map<K,V, Less,Alloc>& other) {
      v_ = other.v_;
      return *this;
   }

   flat_map& operator=(flat_map<K,V, Less,Alloc>&& other) noexcept {
      std::swap(v_, other.v_);
      return *this;
   }

   iterator begin() { return v_.begin(); }
   const_iterator begin() const { return v_.begin(); }
   const_iterator cbegin() const { return v_.cbegin(); }

   iterator end() { return v_.end(); } 
   const_iterator end() const { return v_.end(); } 
   const_iterator cend() const { return v_.cend(); }

   reverse_iterator rbegin() { return v_.rbegin(); }
   const_reverse_iterator rbegin() const { return v_.crbegin(); }
   const_reverse_iterator crbegin() const { return v_.crbegin(); }

   reverse_iterator rend() { return v_.rend(); } 
   const_reverse_iterator rend() const { return v_.rend(); } 
   const_reverse_iterator crend() const { return v_.crend(); }

   bool empty() const {
      return v_.empty(); 
   }

   size_type size() const {
      return v_.size();
   }

   size_type max_size() const {
      return v_.max_size();
   }

   const_iterator upper_bound(const K& key) const {
      value_type t(key, V());
      return std::upper_bound(begin(), end(), t, key_comp);
   }

   const_iterator lower_bound(const K& key) const {
      value_type t(key, V());
      return std::lower_bound(begin(), end(), t, key_comp);
   }

   const_iterator find(const K& key) const {
      auto ret = lower_bound(key);
      if (ret == end() || Less()(key, ret->first)) return end();
      return ret;
   }

   iterator emplace(const K& key, const V& value) {
      iterator it = lower_bound(key);
      if (it != end() && !Less()(key, it->first)) {
         it->second = value;
      } else {
         v_.emplace(it, key, value);
      }
      return it;
   }

   void insert(const_iterator start, const_iterator stop) {
      int incoming_size = std::distance(start, stop);
      int cur_size = size();
      
      if (is_mass_insert(cur_size, incoming_size)) {
         mass_insert(start, stop);
      } else {
         for (const_iterator it = start; it != stop;++it) {
            emplace(it->first, it->second);
         }
      }
   }

   mapped_type& operator[](const key_type& key) {
      iterator it = lower_bound(key);
      if (it != end() && !Less()(key, it->first)) {
         return it->second;
      }
      auto x = v_.emplace(it, key, V());
      return x->second;
   }

private:
   std::vector<value_type,Alloc> v_;

   static bool key_comp(const value_type& el1, const value_type& el2) {
      return Less()(el1.first, el2.first);
   }

   iterator lower_bound(const K& key) {
      value_type t(key, V());
      return std::lower_bound(begin(), end(), t, key_comp);
   }

   int is_mass_insert(size_t current_size, size_t new_size) {
      if (new_size < 4) return false;
      return new_size > current_size;
   }

   void mass_insert(const_iterator start, const_iterator stop) {
      std::vector<value_type, Alloc> t;
      t.reserve(std::distance(start, stop)+v_.size());
      t.insert(t.end(), start, stop);
      t.insert(t.end(), v_.begin(), v_.end());
      std::sort(std::begin(t), std::end(t), key_comp);
      auto it = std::unique(t.begin(), t.end(), 
                            [](const value_type& a, const value_type& b) { 
                              return a.first == b.first; 
                            });
      t.resize(std::distance(t.begin(), it));
      std::swap(v_, t);
   }
};
      
}
