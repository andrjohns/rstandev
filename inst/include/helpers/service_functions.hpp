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
#include <stan/services/sample/fixed_param.hpp>
#include <stan/services/optimize/bfgs.hpp>
#include <stan/services/optimize/lbfgs.hpp>
#include <stan/services/optimize/newton.hpp>
#include <stan/services/experimental/advi/fullrank.hpp>
#include <stan/services/experimental/advi/meanfield.hpp>

#define SERVICE_FUN_DECL(SERVICE_TYPE, FUN_NAME, TYPE_NAME) \
  auto FUN_NAME = [](auto&&... args) { return stan::services::SERVICE_TYPE::FUN_NAME(args...); }; \
  using TYPE_NAME = decltype(FUN_NAME); \
  template <typename F, stan::require_same_t<F, TYPE_NAME>* = nullptr> \
  auto services_fun() { return rstandev::FUN_NAME; } \

namespace rstandev {
  SERVICE_FUN_DECL(sample, hmc_nuts_dense_e_adapt, NutsDenseAdaptT);
  SERVICE_FUN_DECL(sample, hmc_nuts_diag_e_adapt, NutsDiagAdaptT);
  SERVICE_FUN_DECL(sample, hmc_nuts_unit_e_adapt, NutsUnitAdaptT);
  SERVICE_FUN_DECL(sample, hmc_nuts_dense_e, NutsDenseT);
  SERVICE_FUN_DECL(sample, hmc_nuts_diag_e, NutsDiagT);
  SERVICE_FUN_DECL(sample, hmc_nuts_unit_e, NutsUnitT);
  SERVICE_FUN_DECL(sample, hmc_static_dense_e_adapt, StaticDenseAdaptT);
  SERVICE_FUN_DECL(sample, hmc_static_diag_e_adapt, StaticDiagAdaptT);
  SERVICE_FUN_DECL(sample, hmc_static_unit_e_adapt, StaticUnitAdaptT);
  SERVICE_FUN_DECL(sample, hmc_static_dense_e, StaticDenseT);
  SERVICE_FUN_DECL(sample, hmc_static_diag_e, StaticDiagT);
  SERVICE_FUN_DECL(sample, hmc_static_unit_e, StaticUnitT);
  SERVICE_FUN_DECL(sample, fixed_param, FixedParamT);

  SERVICE_FUN_DECL(optimize, bfgs, BFGST);
  SERVICE_FUN_DECL(optimize, lbfgs, LBFGST);
  SERVICE_FUN_DECL(optimize, newton, NewtonT);

  SERVICE_FUN_DECL(experimental::advi, fullrank, FullRankT);
  SERVICE_FUN_DECL(experimental::advi, meanfield, MeanFieldT);

  template <typename T>
  using is_multi_chain = stan::math::disjunction<
    std::is_same<T, NutsDenseAdaptT>,
    std::is_same<T, NutsDiagAdaptT>
  >;

  template <typename T>
  using is_advi = stan::math::disjunction<
    std::is_same<T, FullRankT>,
    std::is_same<T, MeanFieldT>
  >;

  template <typename T>
  using is_optimize = stan::math::disjunction<
    std::is_same<T, BFGST>,
    std::is_same<T, LBFGST>,
    std::is_same<T, NewtonT>
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

  template <typename T>
  using needs_inv_metric = stan::math::disjunction<
    is_dense<T>,
    is_diag<T>
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

  template <typename F, stan::require_all_same_t<F, FixedParamT>* = nullptr>
  inline static auto arg_types() {
    return std::forward_as_tuple(
      StanArg<size_t>("random_seed"),
      StanArg<int>("id"),
      StanArg<double>("init_radius"),
      StanArg<int>("num_samples"),
      StanArg<int>("num_thin"),
      StanArg<int>("refresh")
    );
  }

  template <typename F, stan::require_t<is_advi<F>>* = nullptr>
  inline static auto arg_types() {
    return std::forward_as_tuple(
      StanArg<size_t>("random_seed"),
      StanArg<int>("id"),
      StanArg<double>("init_radius"),
      StanArg<int>("grad_samples"),
      StanArg<int>("elbo_samples"),
      StanArg<int>("max_iterations"),
      StanArg<double>("tol_rel_obj"),
      StanArg<double>("eta"),
      StanArg<bool>("adapt_engaged"),
      StanArg<int>("adapt_iterations"),
      StanArg<int>("eval_elbo"),
      StanArg<int>("output_samples")
    );
  }

  template <typename F, stan::require_all_same_t<F, BFGST>* = nullptr>
  inline static auto arg_types() {
    return std::forward_as_tuple(
      StanArg<size_t>("random_seed"),
      StanArg<int>("id"),
      StanArg<double>("init_radius"),
      StanArg<double>("init_alpha"),
      StanArg<double>("tol_obj"),
      StanArg<double>("tol_rel_obj"),
      StanArg<double>("tol_grad"),
      StanArg<double>("tol_rel_grad"),
      StanArg<double>("tol_param"),
      StanArg<int>("num_iterations"),
      StanArg<bool>("save_iterations"),
      StanArg<int>("refresh")
    );
  }

  template <typename F, stan::require_all_same_t<F, LBFGST>* = nullptr>
  inline static auto arg_types() {
    return std::forward_as_tuple(
      StanArg<size_t>("random_seed"),
      StanArg<int>("id"),
      StanArg<double>("init_radius"),
      StanArg<int>("history_size"),
      StanArg<double>("init_alpha"),
      StanArg<double>("tol_obj"),
      StanArg<double>("tol_rel_obj"),
      StanArg<double>("tol_grad"),
      StanArg<double>("tol_rel_grad"),
      StanArg<double>("tol_param"),
      StanArg<int>("num_iterations"),
      StanArg<bool>("save_iterations"),
      StanArg<int>("refresh")
    );
  }

  template <typename F, stan::require_all_same_t<F, NewtonT>* = nullptr>
  inline static auto arg_types() {
    return std::forward_as_tuple(
      StanArg<size_t>("random_seed"),
      StanArg<int>("id"),
      StanArg<double>("init_radius"),
      StanArg<int>("num_iterations"),
      StanArg<bool>("save_iterations")
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

  template <typename F, stan::require_not_t<is_adapt<F>>* = nullptr,
            stan::require_all_not_same_t<F, FixedParamT>* = nullptr,
            stan::require_not_t<is_optimize<F>>* = nullptr,
            stan::require_not_t<is_advi<F>>* = nullptr>
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
