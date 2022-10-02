#ifndef RSTANDEV_HELPERS_NEW_MODEL_HPP
#define RSTANDEV_HELPERS_NEW_MODEL_HPP

#include <helpers/var_context.hpp>
#include <cpp11/external_pointer.hpp>
#include <iostream>
#include <string>

namespace rstandev {
  template <typename StanModelT>
  cpp11::external_pointer<StanModelT> new_model(std::string rdump_string,
                  unsigned int seed = 0) {
    cpp11::external_pointer<StanModelT> ptr(
        new StanModelT(
            *var_context(rdump_string),
            seed,
            &Rcpp::Rcout)
    );
    return ptr;
  }
}

#endif
