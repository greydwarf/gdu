# gdu 
## FlatMap
gdu::flat_map is an exercise I wrote to learn about implementing containers in C++14.
It hasn't been tested operationally.  If you really want to use something like this, I suggest
using boost's flat_map instead.  That has been tested, and likely has far fewer bugs than this
implementation, which will likely cause you no end of grief.  Really, don't use this.

A flat_map is an associative container that is optimized for only a few entries (less than 30).
It supports unique keys (contains at most one of each key value) and provides for fast retrieval 
of values of another type T based on the keys.

A flat_map satisfies most of the requirements of a container, a reversible
container and an associative container. A flat_map also provides most operations described 
for unique keys. For a flat_map<Key,T> the key_type is Key and the value_type is 
std::pair<Key,T> (unlike std::map<Key, T> which value_type is std::pair<<b>const</b> Key, T>).
flat_map is similar to std::map but it's implemented using a vector instead of a binary tree.
Using a sequential sequence container means that inserting a new element into a flat_map invalidates
previous iterators and references.  Similarly, erasing an invalidates iterators and 
references pointing to elements that come after the erased element.
This container provides random-access iterators.
