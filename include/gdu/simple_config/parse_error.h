#ifndef SC_PARSE_ERROR_H_CGKXQJGH
#define SC_PARSE_ERROR_H_CGKXQJGH

namespace gdu {
   class parse_error: public std::runtime_error {
      ssize_t line_;
      ssize_t offset_;
      std::string expected_;
      std::string found_;
   public:
      parse_error(ssize_t line, ssize_t offset, const char* expected, const char* found):
         std::runtime_error(build_what(line, offset, expected, found)),
         line_(line), offset_(offset), expected_(expected), found_(found) {}
   private:
      static std::string build_what(ssize_t line, ssize_t offset, 
                                    const char* expected, const char* found) {
         char str[1000];
         char short_found[20];
         // Protect against bufer overflows - the "expected" value comes from the
         // client as is probably safe, but the "found" value comes from the 
         // config file and might not be properly checked by the client
         snprintf(short_found, sizeof(short_found), "%s", found);
         sprintf(str, "Parse error on line %zd, at %zd: Expected %s, but found '%s'",
                 line, offset, expected, short_found);
         return  std::string(str);
      }
      ssize_t line() const
      { return line_; }
      ssize_t offset() const
      { return offset_; }
      std::string expected() const
      { return expected_; }
      std::string found() const
      { return found_; }
   };
}

#endif /* end of include guard: SC_PARSE_ERROR_H_CGKXQJGH */
