
#ifndef ERR_h
#define ERR_h

#include <stdexcept>

class out_of_range : public std::logic_error {
public:
  explicit out_of_range (const std::string& what_arg);
  explicit out_of_range (const char* what_arg);
};

#endif