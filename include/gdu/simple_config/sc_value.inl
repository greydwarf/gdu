#ifndef SC_VALUE_H_7GAVULTZ
#define SC_VALUE_H_7GAVULTZ

#include "gdu/simple_config/sc_object.h"
inline gdu::SCValue::SCValue(): type_(SCType::NIL) {}
inline gdu::SCValue::SCValue(const SCValue& other): type_(other.type_) 
{
   switch (type_) {
      case SCType::INT: int_val_ = other.int_val_; break;
      case SCType::FLOAT: float_val_ = other.float_val_; break;
      case SCType::STRING: str_val_ = new std::string(*other.str_val_); break;
      case SCType::DATE: date_val_ = other.date_val_; break;
      case SCType::BOOL: bool_val_ = other.bool_val_; break;
      case SCType::OBJECT: 
         obj_ptr_ = new(gdu::SCObject);
         *obj_ptr_ = *other.obj_ptr_; 
         break;
      case SCType::ARRAY: 
         array_ptr_ = new (ArrayType);
         *array_ptr_ = *other.array_ptr_;
         break;
      default: type_ = SCType::NIL; break;
   }
}
inline gdu::SCValue::SCValue(SCValue&& other): type_(other.type_) 
{
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
      
inline gdu::SCValue::SCValue(int64_t val):type_(SCType::INT),int_val_(val) 
{ }
inline gdu::SCValue::SCValue(int32_t val):type_(SCType::INT),int_val_(val) 
{ }
inline gdu::SCValue::SCValue(int16_t val):type_(SCType::INT),int_val_(val) 
{ }
inline gdu::SCValue::SCValue(int8_t val):type_(SCType::INT),int_val_(val) 
{ }
inline gdu::SCValue::SCValue(uint64_t val):type_(SCType::INT),int_val_(val) 
{ }
inline gdu::SCValue::SCValue(uint32_t val):type_(SCType::INT),int_val_(val) 
{ }
inline gdu::SCValue::SCValue(uint16_t val):type_(SCType::INT),int_val_(val) 
{ }
inline gdu::SCValue::SCValue(uint8_t val):type_(SCType::INT),int_val_(val) 
{ }
inline gdu::SCValue::SCValue(double val):type_(SCType::FLOAT),float_val_(val) 
{ }
inline gdu::SCValue::SCValue(bool val):type_(SCType::BOOL),bool_val_(val) 
{ }
inline gdu::SCValue::SCValue(const std::string& val):type_(SCType::STRING),str_val_(new std::string(val)) 
{ }
inline gdu::SCValue::SCValue(const ArrayType& val):type_(SCType::ARRAY), array_ptr_(new ArrayType(val)) 
{}
inline gdu::SCValue::SCValue(const SCObject& val):type_(SCType::OBJECT), obj_ptr_(new SCObject(val)) 
{}
inline gdu::SCValue::SCValue(int64_t val, bool):type_(SCType::DATE),date_val_(val) 
{ }
inline gdu::SCValue::~SCValue()
{
   switch (type_) {
      case SCType::OBJECT:
      delete obj_ptr_; break;
      case SCType::ARRAY:
      delete array_ptr_; break;
      case SCType::STRING:
      delete str_val_; break;
      default:
      break;
   }
}

inline gdu::SCValue& gdu::SCValue::operator=(const SCValue& other) {
   if (&other == this) { return *this; }
   else if (type_ == SCType::OBJECT) { delete obj_ptr_; }
   else if (type_ == SCType::ARRAY) { delete array_ptr_; }

   type_ = other.type_;
   switch (other.type_) {
      case SCType::INT: int_val_ = other.int_val_; break;
      case SCType::FLOAT: float_val_ = other.float_val_; break;
      case SCType::STRING: str_val_ = new std::string(*other.str_val_); break;
      case SCType::DATE: date_val_ = other.date_val_; break;
      case SCType::BOOL: bool_val_ = other.bool_val_; break;
      case SCType::OBJECT: 
         obj_ptr_ = new(SCObject);
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

inline gdu::SCValue& gdu::SCValue::operator=(SCValue&& other) {
   if (&other == this) { return *this; }

   switch (type_) {
      case SCType::OBJECT:
      delete obj_ptr_; break;
      case SCType::ARRAY:
      delete array_ptr_; break;
      case SCType::STRING:
      delete str_val_; break;
      default:
      break;
   }

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

inline gdu::SCType gdu::SCValue::type() const {
   return type_;
}

inline bool gdu::SCValue::is_valid() const {
   return type_ == SCType::NIL;
}

inline bool gdu::SCValue::is_bool() const {
   return type_ == SCType::BOOL;
}

inline bool gdu::SCValue::is_int() const {
   return type_ == SCType::INT;
}

inline bool gdu::SCValue::is_float() const {
   return type_ == SCType::FLOAT;
}

inline bool gdu::SCValue::is_date() const {
   return type_ == SCType::DATE;
}

inline bool gdu::SCValue::is_string() const {
   return type_ == SCType::STRING;
}

inline bool gdu::SCValue::is_array() const {
   return type_ == SCType::ARRAY;
}

inline bool gdu::SCValue::is_object() const {
   return type_ == SCType::OBJECT;
}

inline int64_t gdu::SCValue::as_int() const {
   if (type_ == SCType::INT) return int_val_;
   else if (type_ == SCType::FLOAT) return static_cast<int64_t>(float_val_);
   else if (type_ == SCType::NIL)
      throw std::invalid_argument("as(): value unset");
   else
      throw std::invalid_argument(
         "as(): type could not be converted to an int");
}

inline int64_t gdu::SCValue::as_int(int64_t def) const {
   if (type_ == SCType::INT) return int_val_;
   else if (type_ == SCType::FLOAT) return static_cast<int64_t>(float_val_);
   else if (type_ == SCType::NIL)
      return def;
   else
      throw std::invalid_argument(
         "as(): type could not be converted to an int");
}

inline double gdu::SCValue::as_float() const {
   if (type_ == SCType::INT) return static_cast<double>(int_val_);
   else if (type_ == SCType::FLOAT) return float_val_;
   else if (type_ == SCType::NIL)
      throw std::invalid_argument("as(): value unset");
   else
      throw std::invalid_argument(
         "as(): type could not be converted to an float");
}

inline double gdu::SCValue::as_float(double def) const {
   if (type_ == SCType::INT) return static_cast<double>(int_val_);
   else if (type_ == SCType::FLOAT) return float_val_;
   else if (type_ == SCType::NIL)
      return def;
   else
      throw std::invalid_argument(
         "as(): type could not be converted to an float");
}

inline int64_t gdu::SCValue::as_date() const {
   if (type_ == SCType::DATE) return date_val_;
   else if (type_ == SCType::NIL)
      throw std::invalid_argument("as(): value unset");
   else
      throw std::invalid_argument(
         "as(): type could not be converted to a date");
}

inline int64_t gdu::SCValue::as_date(int64_t def) const {
   if (type_ == SCType::DATE) return date_val_;
   else if (type_ == SCType::NIL)
      return def;
   else
      throw std::invalid_argument(
         "as(): type could not be converted to a date");
}

inline bool gdu::SCValue::as_bool() const {
   if (type_ == SCType::INT) return int_val_;
   else if (type_ == SCType::BOOL) return bool_val_;
   else if (type_ == SCType::NIL)
      throw std::invalid_argument("as(): value unset");
   else
      throw std::invalid_argument(
         "as(): type could not be converted to a bool");
}

inline double gdu::SCValue::as_bool(double def) const {
   if (type_ == SCType::INT) return static_cast<bool>(int_val_);
   else if (type_ == SCType::BOOL) return bool_val_;
   else if (type_ == SCType::NIL)
      return def;
   else
      throw std::invalid_argument(
         "as(): type could not be converted to a bool");
}

inline gdu::SCValue gdu::SCValue::operator[](size_t el) const {
   if (type_ != SCType::ARRAY) {
      throw std::invalid_argument(
         "[]: type could not be converted to an array");
   }
   assert(el < array_ptr_->size());
   return (*array_ptr_)[el];
}

inline const gdu::SCValue::ArrayType& gdu::SCValue::as_array() const {
   if (type_ != SCType::ARRAY) {
      throw std::invalid_argument(
         "[]: type could not be converted to an array");
   }
   return *array_ptr_;
}

inline const gdu::SCObject& gdu::SCValue::as_object() const {
   if (type_ != SCType::OBJECT) {
      throw std::invalid_argument(
         "[]: type could not be converted to an object");
   }
   return *obj_ptr_;
}

inline const gdu::SCValue& gdu::SCValue::operator[](const std::string& el) const {
   if (type_ != SCType::OBJECT) {
      throw std::invalid_argument(
         "[]: type could not be converted to an array");
   }

   return (*obj_ptr_)[el];
}


inline std::string gdu::SCValue::as_string() const {
   return *str_val_;
}
#endif /* end of include guard: SC_VALUE_H_7GAVULTZ */