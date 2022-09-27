#ifndef RSTANDEV_HELPERS_MAKE_INV_METRIC_HPP
#define RSTANDEV_HELPERS_MAKE_INV_METRIC_HPP

#include <stan/services/util/create_unit_e_dense_inv_metric.hpp>
#include <stan/services/util/create_unit_e_diag_inv_metric.hpp>
#include <stan/services/util/read_dense_inv_metric.hpp>
#include <stan/services/util/read_diag_inv_metric.hpp>

namespace rstandev {
  template <typename ServiceFunctionT,
            stan::require_t<is_dense<ServiceFunctionT>>* = nullptr>
  stan::io::dump make_inv_metric(size_t num_params_r) {
    return stan::services::util::create_unit_e_dense_inv_metric(num_params_r);
  }
  template <typename ServiceFunctionT,
            stan::require_t<is_diag<ServiceFunctionT>>* = nullptr>
  stan::io::dump make_inv_metric(size_t num_params_r) {
    return stan::services::util::create_unit_e_diag_inv_metric(num_params_r);
  }
  template <typename ServiceFunctionT,
            stan::require_t<is_unit<ServiceFunctionT>>* = nullptr>
  stan::io::dump make_inv_metric(size_t num_params_r) {
    return stan::services::util::create_unit_e_diag_inv_metric(num_params_r);
  }
}


#endif
