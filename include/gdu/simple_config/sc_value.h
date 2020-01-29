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
      DATE,
      BOOL,
      OBJECT,
      ARRAY,
      NIL
   };

   class SCValue {
   private:
      SCType type_;
      union {
         int64_t int_val_;
         double  float_val_;
         int64_t date_val_;
         bool    bool_val_;
         std::string* str_val_;
         std::unordered_map<std::string, std::unique_ptr<SCValue>>* obj_ptr_;
         std::vector<std::unique_ptr<SCValue>>* array_ptr_;
      };
      std::string text_;
   public:
      // Right I have these as implicit.  That might change...
      SCValue(): type_(SCType::NIL) {}
      explicit SCValue(int64_t val):type_(SCType::INT),int_val_(val) { }
      explicit SCValue(int32_t val):type_(SCType::INT),int_val_(val) { }
      explicit SCValue(int16_t val):type_(SCType::INT),int_val_(val) { }
      explicit SCValue(int8_t val):type_(SCType::INT),int_val_(val) { }
      explicit SCValue(uint64_t val):type_(SCType::INT),int_val_(val) { }
      explicit SCValue(uint32_t val):type_(SCType::INT),int_val_(val) { }
      explicit SCValue(uint16_t val):type_(SCType::INT),int_val_(val) { }
      explicit SCValue(uint8_t val):type_(SCType::INT),int_val_(val) { }
      explicit SCValue(double val):type_(SCType::FLOAT),float_val_(val) { }
      explicit SCValue(bool val):type_(SCType::BOOL),bool_val_(val) { }
      explicit SCValue(const std::string& val):type_(SCType::STRING),str_val_(new std::string(val)) { }

      // This is a horrible hack, but there it is...
      SCValue(int64_t val, bool):type_(SCType::DATE),date_val_(val) { }

      SCType type() const {
         return type_;
      }

      bool is_valid() const {
         return type_ == SCType::NIL;
      }

      bool is_bool() const {
         return type_ == SCType::BOOL;
      }

      bool is_int() const {
         return type_ == SCType::INT;
      }

      bool is_float() const {
         return type_ == SCType::FLOAT;
      }

      bool is_date() const {
         return type_ == SCType::DATE;
      }

      int64_t as_int() const {
         if (type_ == SCType::INT) return int_val_;
         else if (type_ == SCType::FLOAT) return float_val_;
         else if (type_ == SCType::NIL)
            throw std::invalid_argument("as(): value unset");
         else
            throw std::invalid_argument(
               "as(): type could not be converted to an int");
      }

      int64_t as_int(int64_t def) const {
         if (type_ == SCType::INT) return int_val_;
         else if (type_ == SCType::FLOAT) return float_val_;
         else if (type_ == SCType::NIL)
            return def;
         else
            throw std::invalid_argument(
               "as(): type could not be converted to an int");
      }

      double as_float() const {
         if (type_ == SCType::INT) return int_val_;
         else if (type_ == SCType::FLOAT) return float_val_;
         else if (type_ == SCType::NIL)
            throw std::invalid_argument("as(): value unset");
         else
            throw std::invalid_argument(
               "as(): type could not be converted to an float");
      }

      double as_float(double def) const {
         if (type_ == SCType::INT) return int_val_;
         else if (type_ == SCType::FLOAT) return float_val_;
         else if (type_ == SCType::NIL)
            return def;
         else
            throw std::invalid_argument(
               "as(): type could not be converted to an float");
      }

      int64_t as_date() const {
         if (type_ == SCType::DATE) return date_val_;
         else if (type_ == SCType::NIL)
            throw std::invalid_argument("as(): value unset");
         else
            throw std::invalid_argument(
               "as(): type could not be converted to a date");
      }

      int64_t as_date(int64_t def) const {
         if (type_ == SCType::DATE) return date_val_;
         else if (type_ == SCType::NIL)
            return def;
         else
            throw std::invalid_argument(
               "as(): type could not be converted to a date");
      }

      bool as_bool() const {
         if (type_ == SCType::INT) return int_val_;
         else if (type_ == SCType::BOOL) return bool_val_;
         else if (type_ == SCType::NIL)
            throw std::invalid_argument("as(): value unset");
         else
            throw std::invalid_argument(
               "as(): type could not be converted to a bool");
      }

      double as_bool(double def) const {
         if (type_ == SCType::INT) return int_val_;
         else if (type_ == SCType::BOOL) return bool_val_;
         else if (type_ == SCType::NIL)
            return def;
         else
            throw std::invalid_argument(
               "as(): type could not be converted to a bool");
      }
   };
}
#endif /* end of include guard: SC_VALUE_H_T97XF5AS */
