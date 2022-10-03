#ifndef RSTANDEV_HELPERS_SERVICE_ARG_TYPES_HPP
#define RSTANDEV_HELPERS_SERVICE_ARG_TYPES_HPP

#include <helpers/StanArg.hpp>
#include <helpers/typedefs.hpp>
#include <stan/math/prim/meta.hpp>

namespace rstandev {
  template <typename F, stan::require_any_same_t<F, NutsUnitAdaptT, StaticUnitAdaptT>* = nullptr>
  inline static auto service_arg_types() {
    return std::make_tuple(
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
  inline static auto service_arg_types() {
    return std::make_tuple(
      StanArg<size_t>("random_seed"),
      StanArg<int>("id"),
      StanArg<double>("init_radius"),
      StanArg<int>("num_samples"),
      StanArg<int>("num_thin"),
      StanArg<int>("refresh")
    );
  }

  template <typename F, stan::require_t<is_advi<F>>* = nullptr>
  inline static auto service_arg_types() {
    return std::make_tuple(
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
  inline static auto service_arg_types() {
    return std::make_tuple(
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
  inline static auto service_arg_types() {
    return std::make_tuple(
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
  inline static auto service_arg_types() {
    return std::make_tuple(
      StanArg<size_t>("random_seed"),
      StanArg<int>("id"),
      StanArg<double>("init_radius"),
      StanArg<int>("num_iterations"),
      StanArg<bool>("save_iterations")
    );
  }

  template <typename F, stan::require_t<is_adapt<F>>* = nullptr,
            stan::require_all_not_same_t<F, NutsUnitAdaptT, StaticUnitAdaptT>* = nullptr>
  inline static auto service_arg_types() {
    return std::make_tuple(
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
  inline static auto service_arg_types() {
    return std::make_tuple(
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