#ifndef RSTANDEV_SERVICES_HMC_NUTS_HPP
#define RSTANDEV_SERVICES_HMC_NUTS_HPP

#include <helpers/StanArg.hpp>
#include <helpers/r_interrupt_functor.hpp>
#include <stan/callbacks/stream_logger.hpp>
#include <stan/callbacks/unique_stream_writer.hpp>
#include <stan/services/sample/hmc_nuts_dense_e.hpp>
#include <stan/services/sample/hmc_nuts_dense_e_adapt.hpp>
#include <stan/services/sample/hmc_nuts_diag_e_adapt.hpp>
#include <stan/services/sample/hmc_nuts_diag_e.hpp>

namespace rstandev {
  auto hmc_nuts_dense_e_adapt = [](auto&&... args) { return stan::services::sample::hmc_nuts_dense_e_adapt(args...); };
  auto hmc_nuts_diag_e_adapt = [](auto&&... args) { return stan::services::sample::hmc_nuts_diag_e_adapt(args...); };
  auto hmc_nuts_dense_e = [](auto&&... args) { return stan::services::sample::hmc_nuts_dense_e(args...); };
  auto hmc_nuts_diag_e = [](auto&&... args) { return stan::services::sample::hmc_nuts_diag_e(args...); };

  using NutsDenseAdaptT = decltype(hmc_nuts_dense_e_adapt);
  using NutsDiagAdaptT = decltype(hmc_nuts_diag_e_adapt);
  using NutsDenseT = decltype(hmc_nuts_dense_e);
  using NutsDiagT = decltype(hmc_nuts_diag_e);

  template <typename T>
  using is_multi_chain = stan::math::disjunction<
    std::is_same<T, NutsDenseAdaptT>,
    std::is_same<T, NutsDiagAdaptT>
  >;

  template <typename T>
  using is_not_multi_chain =
    stan::bool_constant<!bool(is_multi_chain<T>::value)>;

  template <typename F,
            stan::require_same_t<F, NutsDenseT>* = nullptr>
  auto services_fun() {
    return rstandev::hmc_nuts_dense_e;
  }
  template <typename F,
            stan::require_same_t<F, NutsDiagT>* = nullptr>
  auto services_fun() {
    return rstandev::hmc_nuts_diag_e;
  }

  template <typename F,
            stan::require_same_t<F, NutsDenseAdaptT>* = nullptr>
  auto services_fun() {
    return rstandev::hmc_nuts_dense_e_adapt;
  }

  template <typename F,
            stan::require_same_t<F, NutsDiagAdaptT>* = nullptr>
  auto services_fun() {
    return rstandev::hmc_nuts_diag_e_adapt;
  }

  template <typename F, stan::require_t<is_multi_chain<F>>* = nullptr>
  inline static auto arg_types() {
    return std::forward_as_tuple(
      StanArg<size_t>("random_seed"),
      StanArg<int>("id"),
      StanArg<double>("init_radius"),
      StanArg<int>("num_warmup"),
      StanArg<int>("num_samples"),
      StanArg<int>("num_thin"),
      StanArg<bool>("save_warmup"),
      StanArg<int>("refresh"),
      StanArg<double>("stepsize"),
      StanArg<double>("stepsize_jitter"),
      StanArg<int>("max_depth"),
      StanArg<double>("delta"),
      StanArg<double>("gamma"),
      StanArg<double>("kappa"),
      StanArg<double>("t0"),
      StanArg<size_t>("init_buffer"),
      StanArg<size_t>("term_buffer"),
      StanArg<size_t>("window")
    );
  }

  template <typename F, stan::require_not_t<is_multi_chain<F>>* = nullptr>
  inline static auto arg_types() {
    return std::forward_as_tuple(
      StanArg<size_t>("random_seed"),
      StanArg<int>("id"),
      StanArg<double>("init_radius"),
      StanArg<int>("num_warmup"),
      StanArg<int>("num_samples"),
      StanArg<int>("num_thin"),
      StanArg<bool>("save_warmup"),
      StanArg<int>("refresh"),
      StanArg<double>("stepsize"),
      StanArg<double>("stepsize_jitter"),
      StanArg<int>("max_depth")
    );
  }

} // namespace rstandev

#endif
