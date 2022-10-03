#ifndef RSTANDEV_HELPERS_SERVICE_FUNCTION_WRAPPERS_HPP
#define RSTANDEV_HELPERS_SERVICE_FUNCTION_WRAPPERS_HPP

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
} // namespace rstandev

#endif
