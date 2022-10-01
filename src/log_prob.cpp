#include <stan/model/model_base.hpp>
#include <stan/model/log_prob_grad.hpp>
#include <cpp11/external_pointer.hpp>
#include <cpp11/doubles.hpp>
#include <vector>

[[cpp11::register]]
double log_prob(SEXP ext_model_ptr, std::vector<double> upars) {
  cpp11::external_pointer<stan::model::model_base> ptr(ext_model_ptr);
  std::vector<int> params_i;
  return ptr->log_prob<false, true>(upars, params_i, &std::cout);
}

[[cpp11::register]]
cpp11::doubles grad_log_prob(SEXP ext_model_ptr, std::vector<double> upars) {
  cpp11::external_pointer<stan::model::model_base> ptr(ext_model_ptr);
  std::vector<int> params_i;
  std::vector<double> gradients;

  double lp = stan::model::log_prob_grad<false, true>(
    *ptr.get(), upars, params_i, gradients);
  cpp11::writable::doubles grad_rtn = std::move(gradients);
  grad_rtn.attr("log_prob") = lp;
  return grad_rtn;
}
