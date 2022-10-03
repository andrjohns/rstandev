#ifndef RSTANDEV_HELPERS_TYPEDEFS_HPP
#define RSTANDEV_HELPERS_TYPEDEFS_HPP

#include <helpers/service_function_wrappers.hpp>

namespace rstandev {

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

  using context_vector = std::vector<std::shared_ptr<stan::io::var_context>>;

} // namespace rstandev


#endif