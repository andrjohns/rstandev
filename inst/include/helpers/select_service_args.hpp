#ifndef RSTANDEV_HELPERS_SELECT_SERVICE_ARGS_HPP
#define RSTANDEV_HELPERS_SELECT_SERVICE_ARGS_HPP

#include <helpers/typedefs.hpp>

namespace rstandev {
  template <typename ServiceFunT, typename ModelT, typename InitContextsT, typename InvMetricsT,
            stan::require_not_t<needs_inv_metric<ServiceFunT>>* = nullptr>
  decltype(auto) select_service_args_head(ModelT&& model, InitContextsT&& init_contexts, InvMetricsT&& inv_metrics) {
    return std::forward_as_tuple(model, init_contexts);
  }

  template <typename ServiceFunT, typename ModelT, typename InitContextsT, typename InvMetricsT,
            stan::require_t<needs_inv_metric<ServiceFunT>>* = nullptr>
  decltype(auto) select_service_args_head(ModelT&& model, InitContextsT&& init_contexts, InvMetricsT&& inv_metrics) {
    return std::forward_as_tuple(model, init_contexts, inv_metrics);
  }

  template <typename ServiceFunT, typename InterruptT, typename LoggerT, typename InitWritersT,
            typename SampleWritersT, typename DiagnosticWritersT,
            stan::require_not_t<is_optimize<ServiceFunT>>* = nullptr>
  decltype(auto) select_service_args_tail(InterruptT&& interrupt, LoggerT&& logger, InitWritersT&& init_writers,
                               SampleWritersT&& sample_writers, DiagnosticWritersT&& diagnostic_writers) {
    return std::forward_as_tuple(interrupt, logger, init_writers, sample_writers, diagnostic_writers);
  }

  template <typename ServiceFunT, typename InterruptT, typename LoggerT, typename InitWritersT,
            typename SampleWritersT, typename DiagnosticWritersT,
            stan::require_t<is_optimize<ServiceFunT>>* = nullptr>
  decltype(auto) select_service_args_tail(InterruptT&& interrupt, LoggerT&& logger, InitWritersT&& init_writers,
                               SampleWritersT&& sample_writers, DiagnosticWritersT&& diagnostic_writers) {
    return std::forward_as_tuple(interrupt, logger, init_writers, sample_writers);
  }
} // namespace rstandev


#endif