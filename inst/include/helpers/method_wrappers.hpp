#ifndef RSTANDEV_HELPERS_METHOD_WRAPPERS_HPP
#define RSTANDEV_HELPERS_METHOD_WRAPPERS_HPP

#include <helpers/prepare_args.hpp>
#include <helpers/method_typedefs.hpp>
#include <unordered_map>

namespace rstandev {
  template <typename F>
  int apply_fun(cpp11::list args) {
    return stan::math::apply(
      services_fun<F>(),
      std::forward<decltype(prepare_args<F>(args))>(prepare_args<F>(args)));
  }

  auto lookup_function(std::string fun_name) {
    static const std::unordered_map<std::string, std::function<int(cpp11::list)>> methods_index = {
      {"hmc_nuts_dense_e_adapt", apply_fun<NutsDenseAdaptT>},
      {"hmc_nuts_dense_e", apply_fun<NutsDenseT>}
    };

    return methods_index.find(fun_name)->second;
  }
}

#endif
