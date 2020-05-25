#ifndef SC_VALUE_H_T97XF5AS
#define SC_VALUE_H_T97XF5AS
#include <unordered_map>
#include <vector>
#include <memory>
#include <cassert>

namespace gdu {
   class SCObject;
   enum class SCType {
      INT,
      FLOAT,
      STRING,
      DATE,
      BOOL,
      OBJECT,
      ARRAY,
      NIL
   };

   class SCValue {
   private:
      using ArrayType = std::vector<SCValue>;
      SCType type_;
      union {
         int64_t int_val_;
         double  float_val_;
         int64_t date_val_;
         bool    bool_val_;
         std::string* str_val_;
         SCObject* obj_ptr_;
         ArrayType* array_ptr_;
      };
      std::string text_;
   public:
      SCValue();
      SCValue(const SCValue& other);
      SCValue(SCValue&& other);

      // Right now I have these as explicit.  That might change...
      explicit SCValue(int64_t val);
      explicit SCValue(int32_t val);
      explicit SCValue(int16_t val);
      explicit SCValue(int8_t val);
      explicit SCValue(uint64_t val);
      explicit SCValue(uint32_t val);
      explicit SCValue(uint16_t val);
      explicit SCValue(uint8_t val);
      explicit SCValue(double val);
      explicit SCValue(bool val);
      explicit SCValue(const std::string& val);
      explicit SCValue(const ArrayType& val);
      explicit SCValue(const SCObject& val);

      // This is a horrible hack, but it is no worse than the pre/post increment operator
      // in the C++ standard
      SCValue(int64_t val, bool);
      ~SCValue();

      SCValue& operator=(const SCValue& other);
      SCValue& operator=(SCValue&& other);

      SCType type() const ;
      bool is_valid() const;
      bool is_bool() const;
      bool is_int() const;
      bool is_float() const;
      bool is_date() const;
      bool is_string() const;
      bool is_array() const;
      bool is_object() const;

      int64_t as_int() const;
      int64_t as_int(int64_t def) const;
      double as_float() const;
      double as_float(double def) const;
      int64_t as_date() const;
      int64_t as_date(int64_t def) const;
      bool as_bool() const;
      double as_bool(double def) const;
      SCValue operator[](size_t el) const;
      const ArrayType& as_array() const;
      const SCObject& as_object() const;
      const SCValue& operator[](const std::string& el) const;

      std::string str() const;
   };
}
#endif /* end of include guard: SC_VALUE_H_T97XF5AS */

#include "gdu/simple_config/sc_value.inl"

