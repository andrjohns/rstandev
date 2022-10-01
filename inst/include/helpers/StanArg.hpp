#ifndef RSTANDEV_HELPERS_STANARG_HPP
#define RSTANDEV_HELPERS_STANARG_HPP

#include <string>

namespace rstandev {
  template <typename T>
  class StanArg {
    public:
      T dummy;
      std::string arg_name;

      StanArg(std::string name) {
        arg_name = name;
      }
  };
}

#endif
