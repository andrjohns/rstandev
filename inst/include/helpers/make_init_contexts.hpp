#ifndef RSTANDEV_HELPERS_MAKE_INIT_CONTEXTS_HPP
#define RSTANDEV_HELPERS_MAKE_INIT_CONTEXTS_HPP

#include <helpers/var_context.hpp>
#include <cpp11/list.hpp>
#include <cpp11/external_pointer.hpp>
#include <cpp11/as.hpp>
#include <vector>
#include <memory>


namespace rstandev {
  cpp11::external_pointer<std::vector<std::shared_ptr<stan::io::var_context>>>
  make_init_contexts(cpp11::writable::list args) {
    std::string rdump_init = cpp11::as_cpp<std::string>(args["rdump_init"]);
    int num_chains = cpp11::as_cpp<int>(args["num_chains"]);

    cpp11::external_pointer<std::vector<std::shared_ptr<stan::io::var_context>>> init_contexts(
      new std::vector<std::shared_ptr<stan::io::var_context>>
    );

    for (int i = 1; i < num_chains; ++i) {
      init_contexts->push_back(var_context(rdump_init));
    }

    return init_contexts;
  }
}

#endif
