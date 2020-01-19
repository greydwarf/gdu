#ifndef SC_VALUE_H_T97XF5AS
#define SC_VALUE_H_T97XF5AS
#include <unordered_map>
#include <vector>
#include <memory>

namespace gdu {
   enum class SCType {
      INT,
      FLOAT,
      STRING,
      TIME,
      BOOL,
      OBJECT,
      ARRAY,
      NIL
   };

   class SCValue;
   template <typename T>
   class as_helper { 
   public:
      static T as(const SCValue& v);
      static T as(const SCValue& v, const T& def);
   };

   class SCValue {
      template<typename T> friend class as_helper;
   private:
      union {
         int64_t int_val_;
         double  float_val_;
         int64_t date_val_;
         bool    bool_val_;
         std::unordered_map<std::string, std::unique_ptr<SCValue>>* obj_ptr_;
         std::vector<std::unique_ptr<SCValue>>* array_ptr_;
      };
      SCType type_;
      std::string text_;
   public:
      SCType type() const {
         return type_;
      }
      template <typename T> T as() {
         return as_helper<T>::as(*this);
      }
   private:
      void set_int(int64_t val) {
         type_ = SCType::INT;
         int_val_ = val;
      }
   };

#ifdef GDU_SCVALUE_AS_INT_T97XF5AS
#  error GDU_SCVALUE_AS_INT_T97XF5AS Must be Undefined
#endif

#define GDU_SCVALUE_AS_INT_T97XF5AS(NUM_TYPE)                   \
   template <> class as_helper<NUM_TYPE> {                      \
   public:                                                      \
      static NUM_TYPE as(const SCValue& v) {                          \
         if (v.type_ == SCType::INT) return v.int_val_;             \
         else if (v.type_ == SCType::FLOAT) return v.float_val_;    \
         else if (v.type_ == SCType::NIL)                         \
            throw std::invalid_argument("as(): value unset");   \
         else                                                   \
            throw std::invalid_argument(                        \
               "as(): type could not be converted to an int");  \
      }                                                         \
      static NUM_TYPE as(const SCValue& v, const NUM_TYPE& def) {       \
         if (v.type_ == SCType::INT) return v.int_val_;             \
         else if (v.type_ == SCType::FLOAT) return v.float_val_;    \
         else if (v.type_ == SCType::NIL)                         \
            return def;                                         \
         else                                                   \
            throw std::invalid_argument(                        \
               "as(): type could not be converted to an int");  \
      }                                                         \
   }
   
GDU_SCVALUE_AS_INT_T97XF5AS(int8_t);
GDU_SCVALUE_AS_INT_T97XF5AS(uint8_t);
GDU_SCVALUE_AS_INT_T97XF5AS(int16_t);
GDU_SCVALUE_AS_INT_T97XF5AS(uint16_t);
GDU_SCVALUE_AS_INT_T97XF5AS(int32_t);
GDU_SCVALUE_AS_INT_T97XF5AS(uint32_t);
GDU_SCVALUE_AS_INT_T97XF5AS(int64_t);
GDU_SCVALUE_AS_INT_T97XF5AS(uint64_t);
GDU_SCVALUE_AS_INT_T97XF5AS(double);
GDU_SCVALUE_AS_INT_T97XF5AS(float);

#undef GDU_SCVALUE_AS_INT_T97XF5AS
}
#endif /* end of include guard: SC_VALUE_H_T97XF5AS */
