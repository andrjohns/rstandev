#ifndef RSTANDEV_SERVICES_HMC_NUTS_HPP
#define RSTANDEV_SERVICES_HMC_NUTS_HPP

#include <helpers/StanArg.hpp>
#include <stan/callbacks/stream_logger.hpp>
#include <stan/callbacks/unique_stream_writer.hpp>
#include <stan/services/sample/hmc_nuts_dense_e_adapt.hpp>
#include <stan/services/sample/hmc_nuts_diag_e_adapt.hpp>
#include <stan/services/sample/hmc_nuts_unit_e_adapt.hpp>
#include <stan/services/sample/hmc_nuts_dense_e.hpp>
#include <stan/services/sample/hmc_nuts_diag_e.hpp>
#include <stan/services/sample/hmc_nuts_unit_e.hpp>
#include <stan/services/sample/hmc_static_dense_e_adapt.hpp>
#include <stan/services/sample/hmc_static_diag_e_adapt.hpp>
#include <stan/services/sample/hmc_static_unit_e_adapt.hpp>
#include <stan/services/sample/hmc_static_dense_e.hpp>
#include <stan/services/sample/hmc_static_diag_e.hpp>
#include <stan/services/sample/hmc_static_unit_e.hpp>

#define SERVICE_FUN_DECL(FUN_NAME, TYPE_NAME) \
  auto FUN_NAME = [](auto&&... args) { return stan::services::sample::FUN_NAME(args...); }; \
  using TYPE_NAME = decltype(FUN_NAME); \
  template <typename F, stan::require_same_t<F, TYPE_NAME>* = nullptr> \
  auto services_fun() { return rstandev::FUN_NAME; } \


namespace rstandev {
  SERVICE_FUN_DECL(hmc_nuts_dense_e_adapt, NutsDenseAdaptT);
  SERVICE_FUN_DECL(hmc_nuts_diag_e_adapt, NutsDiagAdaptT);
  SERVICE_FUN_DECL(hmc_nuts_unit_e_adapt, NutsUnitAdaptT);
  SERVICE_FUN_DECL(hmc_nuts_dense_e, NutsDenseT);
  SERVICE_FUN_DECL(hmc_nuts_diag_e, NutsDiagT);
  SERVICE_FUN_DECL(hmc_nuts_unit_e, NutsUnitT);
  SERVICE_FUN_DECL(hmc_static_dense_e_adapt, StaticDenseAdaptT);
  SERVICE_FUN_DECL(hmc_static_diag_e_adapt, StaticDiagAdaptT);
  SERVICE_FUN_DECL(hmc_static_unit_e_adapt, StaticUnitAdaptT);
  SERVICE_FUN_DECL(hmc_static_dense_e, StaticDenseT);
  SERVICE_FUN_DECL(hmc_static_diag_e, StaticDiagT);
  SERVICE_FUN_DECL(hmc_static_unit_e, StaticUnitT);

  template <typename T>
  using is_multi_chain = stan::math::disjunction<
    std::is_same<T, NutsDenseAdaptT>,
    std::is_same<T, NutsDiagAdaptT>
  >;

  template <typename T>
  using is_dense = stan::math::disjunction<
    std::is_same<T, NutsDenseAdaptT>,
    std::is_same<T, NutsDenseT>,
    std::is_same<T, StaticDenseAdaptT>,
    std::is_same<T, StaticDenseT>
  >;

  template <typename T>
  using is_diag = stan::math::disjunction<
    std::is_same<T, NutsDiagAdaptT>,
    std::is_same<T, NutsDiagT>,
    std::is_same<T, StaticDiagAdaptT>,
    std::is_same<T, StaticDiagT>
  >;

  template <typename T>
  using is_unit = stan::math::disjunction<
    std::is_same<T, NutsUnitAdaptT>,
    std::is_same<T, NutsUnitT>,
    std::is_same<T, StaticUnitAdaptT>,
    std::is_same<T, StaticUnitT>
  >;

  template <typename T>
  using is_adapt = stan::math::disjunction<
    std::is_same<T, NutsDenseAdaptT>,
    std::is_same<T, NutsDiagAdaptT>,
    std::is_same<T, NutsUnitAdaptT>,
    std::is_same<T, StaticDenseAdaptT>,
    std::is_same<T, StaticDiagAdaptT>,
    std::is_same<T, StaticUnitAdaptT>
  >;

  template <typename F, stan::require_any_same_t<F, NutsUnitAdaptT, StaticUnitAdaptT>* = nullptr>
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
      StanArg<double>("t0")
    );
  }

  template <typename F, stan::require_t<is_adapt<F>>* = nullptr,
            stan::require_all_not_same_t<F, NutsUnitAdaptT, StaticUnitAdaptT>* = nullptr>
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

  template <typename F, stan::require_not_t<is_adapt<F>>* = nullptr>
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
