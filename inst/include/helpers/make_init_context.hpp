#ifndef RSTANDEV_HELPERS_MAKE_INIT_CONTEXT_HPP
#define RSTANDEV_HELPERS_MAKE_INIT_CONTEXT_HPP

#include <helpers/var_context.hpp>
#include <cpp11/list.hpp>
#include <cpp11/external_pointer.hpp>
#include <cpp11/as.hpp>
#include <vector>
#include <memory>


namespace rstandev {
  template <typename ServiceFunctionT>
  cpp11::external_pointer<std::shared_ptr<stan::io::var_context>>
  make_init_context(cpp11::writable::list args) {
    std::string rdump_init = cpp11::as_cpp<std::string>(args["rdump_init"]);

    return cpp11::external_pointer<std::shared_ptr<stan::io::var_context>>(
      new std::shared_ptr<stan::io::var_context>(var_context(rdump_init))
    );
  }
}

#endif
