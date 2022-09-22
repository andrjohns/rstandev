#ifndef RSTANDEV_HELPERS_METHOD_TYPEDEFS_HPP
#define RSTANDEV_HELPERS_METHOD_TYPEDEFS_HPP

#include <stan/services/sample/hmc_nuts_dense_e_adapt.hpp>
#include <stan/services/sample/hmc_nuts_dense_e.hpp>

namespace rstandev {
  auto hmc_nuts_dense_e_adapt = [](auto&&... args) { return stan::services::sample::hmc_nuts_dense_e_adapt(args...); };
  auto hmc_nuts_dense_e = [](auto&&... args) { return stan::services::sample::hmc_nuts_dense_e(args...); };
  using NutsDenseAdaptT = decltype(hmc_nuts_dense_e_adapt);
  using NutsDenseT = decltype(hmc_nuts_dense_e);

  template <typename F,
            stan::require_same_t<F, NutsDenseAdaptT>* = nullptr>
  auto services_fun() {
    return rstandev::hmc_nuts_dense_e_adapt;
  }
  template <typename F,
            stan::require_same_t<F, NutsDenseT>* = nullptr>
  auto services_fun() {
    return rstandev::hmc_nuts_dense_e;
  }
} // namespace rstandev

#endif
