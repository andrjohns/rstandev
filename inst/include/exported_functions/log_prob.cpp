#include <helpers/get_model_ptr.hpp>
#include <stan/model/log_prob_grad.hpp>
#include <cpp11/external_pointer.hpp>
#include <cpp11/as.hpp>
#include <cpp11/list.hpp>
#include <cpp11/doubles.hpp>
#include <vector>

[[cpp11::register]]
double log_prob(cpp11::list args_list) {
  cpp11::external_pointer<stan_model> ptr = rstandev::get_model_ptr<stan_model>(args_list);
  std::vector<double> upars
    = cpp11::as_cpp<std::vector<double>>(args_list["upars"]);
  std::vector<int> params_i;
  return ptr->log_prob<false, true>(upars, params_i);
}

[[cpp11::register]]
cpp11::writable::doubles grad_log_prob(cpp11::list args_list) {
  cpp11::external_pointer<stan_model> ptr = rstandev::get_model_ptr<stan_model>(args_list);
  std::vector<double> upars
    = cpp11::as_cpp<std::vector<double>>(args_list["upars"]);
  std::vector<int> params_i;
  std::vector<double> gradients;

  double lp = stan::model::log_prob_grad<false, true>(
    *ptr.get(), upars, params_i, gradients);
  cpp11::writable::doubles grad_rtn = std::move(gradients);
  grad_rtn.attr("log_prob") = lp;
  return grad_rtn;
}
