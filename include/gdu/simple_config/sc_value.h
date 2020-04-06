#ifndef SC_VALUE_H_T97XF5AS
#define SC_VALUE_H_T97XF5AS
#include <unordered_map>
#include <vector>
#include <memory>
#include <cassert>

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
      using ArrayType = std::vector<SCValue>;
      SCType type_;
      union {
         int64_t int_val_;
         double  float_val_;
         int64_t date_val_;
         bool    bool_val_;
         std::string* str_val_;
         std::unordered_map<std::string, SCValue>* obj_ptr_;
         ArrayType* array_ptr_;
      };
      std::string text_;
   public:
      SCValue(): type_(SCType::NIL) {}
      SCValue(const SCValue& other): type_(other.type_) {
         switch (type_) {
            case SCType::INT: int_val_ = other.int_val_; break;
            case SCType::FLOAT: float_val_ = other.float_val_; break;
            case SCType::STRING: str_val_ = other.str_val_; break;
            case SCType::DATE: date_val_ = other.date_val_; break;
            case SCType::BOOL: bool_val_ = other.bool_val_; break;
            case SCType::OBJECT: 
               obj_ptr_ = new(std::unordered_map<std::string, SCValue>);
               *obj_ptr_ = *other.obj_ptr_; 
               break;
            case SCType::ARRAY: 
               array_ptr_ = new (ArrayType);
               *array_ptr_ = *other.array_ptr_;
               break;
            default: type_ = SCType::NIL; break;
         }
      }
      SCValue(SCValue&& other): type_(other.type_) {
         switch (type_) {
            case SCType::INT: int_val_ = other.int_val_; break;
            case SCType::FLOAT: float_val_ = other.float_val_; break;
            case SCType::STRING: str_val_ = other.str_val_; break;
            case SCType::DATE: date_val_ = other.date_val_; break;
            case SCType::BOOL: bool_val_ = other.bool_val_; break;
            case SCType::OBJECT: obj_ptr_ = other.obj_ptr_; break;
            case SCType::ARRAY: array_ptr_ = other.array_ptr_; break;
            default: type_ = SCType::NIL; break;
         }
         other.type_ = SCType::NIL;
      }

      // Right now I have these as explicit.  That might change...
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
      explicit SCValue(const ArrayType& val):type_(SCType::ARRAY), array_ptr_(new ArrayType(val)) 
      {}

      // This is a horrible hack, but it is no worse than the pre/post increment operator
      // in the C++ standard
      SCValue(int64_t val, bool):type_(SCType::DATE),date_val_(val) { }

      SCValue& operator=(const SCValue& other) {
         if (&other == this) { return *this; }
         else if (type_ == SCType::OBJECT) { delete obj_ptr_; }
         else if (type_ == SCType::ARRAY) { delete array_ptr_; }

         type_ = other.type_;
         switch (other.type_) {
            case SCType::INT: int_val_ = other.int_val_; break;
            case SCType::FLOAT: float_val_ = other.float_val_; break;
            case SCType::STRING: str_val_ = other.str_val_; break;
            case SCType::DATE: date_val_ = other.date_val_; break;
            case SCType::BOOL: bool_val_ = other.bool_val_; break;
            case SCType::OBJECT: 
               obj_ptr_ = new(std::unordered_map<std::string, SCValue>);
               *obj_ptr_ = *other.obj_ptr_; 
               break;
            case SCType::ARRAY: 
               array_ptr_ = new (ArrayType);
               *array_ptr_ = *other.array_ptr_;
               break;
            default: type_ = SCType::NIL; break;
         }
         return *this;
      }

      SCValue& operator=(SCValue&& other) {
         if (&other == this) { return *this; }
         else if (type_ == SCType::OBJECT) { delete obj_ptr_; }
         else if (type_ == SCType::ARRAY) { delete array_ptr_; }

         type_ = other.type_;
         switch (other.type_) {
            case SCType::INT: int_val_ = other.int_val_; break;
            case SCType::FLOAT: float_val_ = other.float_val_; break;
            case SCType::STRING: str_val_ = other.str_val_; break;
            case SCType::DATE: date_val_ = other.date_val_; break;
            case SCType::BOOL: bool_val_ = other.bool_val_; break;
            case SCType::OBJECT: obj_ptr_ = other.obj_ptr_; break;
            case SCType::ARRAY: array_ptr_ = other.array_ptr_; break;
            default: type_ = SCType::NIL; break;
         }
         other.type_ = SCType::NIL;
         return *this;
      }

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

      bool is_string() const {
         return type_ == SCType::STRING;
      }

      bool is_array() const {
         return type_ == SCType::ARRAY;
      }

      int64_t as_int() const {
         if (type_ == SCType::INT) return int_val_;
         else if (type_ == SCType::FLOAT) return static_cast<int64_t>(float_val_);
         else if (type_ == SCType::NIL)
            throw std::invalid_argument("as(): value unset");
         else
            throw std::invalid_argument(
               "as(): type could not be converted to an int");
      }

      int64_t as_int(int64_t def) const {
         if (type_ == SCType::INT) return int_val_;
         else if (type_ == SCType::FLOAT) return static_cast<int64_t>(float_val_);
         else if (type_ == SCType::NIL)
            return def;
         else
            throw std::invalid_argument(
               "as(): type could not be converted to an int");
      }

      double as_float() const {
         if (type_ == SCType::INT) return static_cast<double>(int_val_);
         else if (type_ == SCType::FLOAT) return float_val_;
         else if (type_ == SCType::NIL)
            throw std::invalid_argument("as(): value unset");
         else
            throw std::invalid_argument(
               "as(): type could not be converted to an float");
      }

      double as_float(double def) const {
         if (type_ == SCType::INT) return static_cast<double>(int_val_);
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
         if (type_ == SCType::INT) return static_cast<bool>(int_val_);
         else if (type_ == SCType::BOOL) return bool_val_;
         else if (type_ == SCType::NIL)
            return def;
         else
            throw std::invalid_argument(
               "as(): type could not be converted to a bool");
      }

      SCValue operator[](size_t el) const {
         if (type_ != SCType::ARRAY) {
            throw std::invalid_argument(
               "[]: type could not be converted to an array");
         }
         assert(el < array_ptr_->size());
         return (*array_ptr_)[el];
      }

      const ArrayType& as_array() const {
         if (type_ != SCType::ARRAY) {
            throw std::invalid_argument(
               "[]: type could not be converted to an array");
         }
         return *array_ptr_;
      }


      std::string str() const {
         return *str_val_;
      }
   };
}
#endif /* end of include guard: SC_VALUE_H_T97XF5AS */
