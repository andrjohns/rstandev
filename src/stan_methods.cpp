
#include <helpers/apply_service_function.hpp>
#include <cpp11/list.hpp>
#include <cpp11/strings.hpp>
#include <tbb/global_control.h>
#include <stan/model/model_base.hpp>
#include <stan/model/log_prob_grad.hpp>
#include <helpers/var_context.hpp>
#include <cpp11/external_pointer.hpp>
#include <cpp11/as.hpp>
#include <cpp11/list.hpp>
#include <cpp11/doubles.hpp>
#include <cpp11/strings.hpp>
#include <cpp11/named_arg.hpp>
#include <R.h>
#include <Rcpp/iostream/Rstreambuf.h>
#include <boost/random/additive_combine.hpp>
#include <vector>

[[cpp11::register]]
double log_prob(SEXP ext_model_ptr, std::vector<double> upars) {
  cpp11::external_pointer<stan::model::model_base> ptr(ext_model_ptr);
  std::vector<int> params_i;
  return ptr->log_prob<false, true>(upars, params_i, &Rcpp::Rcout);
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

[[cpp11::register]]
std::vector<std::string> get_param_names(SEXP ext_model_ptr) {
  cpp11::external_pointer<stan::model::model_base> ptr(ext_model_ptr);
  std::vector<std::string> names;
  ptr->get_param_names(names);
  return names;
}

[[cpp11::register]]
cpp11::list get_dims(SEXP ext_model_ptr) {
  cpp11::external_pointer<stan::model::model_base> ptr(ext_model_ptr);
  std::vector<std::vector<size_t>> dims;
  ptr->get_dims(dims);
  std::vector<std::string> names;
  ptr->get_param_names(names);

  cpp11::writable::list dims_list;

  for (int i = 0; i < names.size(); ++i) {
    dims_list.push_back({
      cpp11::named_arg(names[i].data()) = dims[i]
    });
  }
  return dims_list;
}

[[cpp11::register]]
std::vector<double> unconstrain_pars(SEXP ext_model_ptr, std::string pars) {
  cpp11::external_pointer<stan::model::model_base> ptr(ext_model_ptr);
  std::shared_ptr<stan::io::var_context> init_context
    = rstandev::var_context(pars);
  std::vector<int> params_i;
  std::vector<double> vars;
  ptr->transform_inits(*init_context.get(), params_i, vars, &Rcpp::Rcout);
  return vars;
}

[[cpp11::register]]
std::vector<double> constrain_pars(SEXP ext_model_ptr, std::vector<double> upars) {
  cpp11::external_pointer<stan::model::model_base> ptr(ext_model_ptr);
  std::vector<int> params_i;
  std::vector<double> vars;
  boost::ecuyer1988 base_rng(static_cast<boost::uint32_t>(std::time(0)));

  ptr->write_array(base_rng, upars, params_i, vars, false, false);
  return vars;
}

[[cpp11::register]]
int stan_methods_wrapper(std::string name, cpp11::list args) {
  static const std::map<std::string, std::function<int(cpp11::list)>> methods_index = {
    {"hmc_nuts_dense_e_adapt", rstandev::apply_service_function<rstandev::NutsDenseAdaptT>},
    {"hmc_nuts_dense_e", rstandev::apply_service_function<rstandev::NutsDenseT>},
    {"hmc_nuts_diag_e_adapt", rstandev::apply_service_function<rstandev::NutsDiagAdaptT>},
    {"hmc_nuts_diag_e", rstandev::apply_service_function<rstandev::NutsDiagT>},
    {"hmc_nuts_unit_e_adapt", rstandev::apply_service_function<rstandev::NutsUnitAdaptT>},
    {"hmc_nuts_unit_e", rstandev::apply_service_function<rstandev::NutsUnitT>},
    {"hmc_static_dense_e_adapt", rstandev::apply_service_function<rstandev::StaticDenseAdaptT>},
    {"hmc_static_dense_e", rstandev::apply_service_function<rstandev::StaticDenseT>},
    {"hmc_static_diag_e_adapt", rstandev::apply_service_function<rstandev::StaticDiagAdaptT>},
    {"hmc_static_diag_e", rstandev::apply_service_function<rstandev::StaticDiagT>},
    {"hmc_static_unit_e_adapt", rstandev::apply_service_function<rstandev::StaticUnitAdaptT>},
    {"hmc_static_unit_e", rstandev::apply_service_function<rstandev::StaticUnitT>},
    {"fixed_param", rstandev::apply_service_function<rstandev::FixedParamT>},

    {"bfgs", rstandev::apply_service_function<rstandev::BFGST>},
    {"lbfgs", rstandev::apply_service_function<rstandev::LBFGST>},
    {"newton", rstandev::apply_service_function<rstandev::NewtonT>},

    {"fullrank", rstandev::apply_service_function<rstandev::FullRankT>},
    {"meanfield", rstandev::apply_service_function<rstandev::MeanFieldT>}

  };
  stan::math::init_threadpool_tbb(cpp11::as_cpp<size_t>(args["num_threads"]));
  return methods_index.at(name)(args);
}
