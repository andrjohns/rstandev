#include <helpers/get_model_ptr.hpp>
#include <helpers/var_context.hpp>
#include <cpp11/external_pointer.hpp>
#include <cpp11/as.hpp>
#include <cpp11/list.hpp>
#include <cpp11/doubles.hpp>
#include <cpp11/strings.hpp>
#include <cpp11/named_arg.hpp>
#include <boost/random/additive_combine.hpp>
#include <vector>

[[cpp11::register]]
cpp11::strings get_param_names(cpp11::list args_list) {
  cpp11::external_pointer<stan_model> ptr = rstandev::get_model_ptr<stan_model>(args_list);
  std::vector<std::string> names;
  ptr->get_param_names(names);
  cpp11::writable::strings ret = std::move(names);
  return ret;
}

[[cpp11::register]]
cpp11::list get_dims(cpp11::list args_list) {
  cpp11::external_pointer<stan_model> ptr = rstandev::get_model_ptr<stan_model>(args_list);
  std::vector<std::vector<size_t>> dims;
  ptr->get_dims(dims);
  std::vector<std::string> names = cpp11::as_cpp<std::vector<std::string>>(get_param_names(args_list));
  cpp11::writable::list dims_list;

  for (int i = 0; i < names.size(); ++i) {
    dims_list.push_back({
      cpp11::named_arg(names[i].data()) = dims[i]
    });
  }
  return dims_list;
}

[[cpp11::register]]
cpp11::doubles unconstrain_pars(cpp11::list args_list) {
  cpp11::external_pointer<stan_model> ptr = rstandev::get_model_ptr<stan_model>(args_list);
  std::string init_string = cpp11::as_cpp<std::string>(args_list["init_string"]);
  std::shared_ptr<stan::io::var_context> init_context
    = rstandev::var_context(init_string);
  std::vector<int> params_i;
  std::vector<double> vars;
  ptr->transform_inits(*init_context.get(), params_i, vars);
  cpp11::writable::doubles ret = std::move(vars);
  return ret;
}

[[cpp11::register]]
cpp11::doubles constrain_pars(cpp11::list args_list) {
  cpp11::external_pointer<stan_model> ptr = rstandev::get_model_ptr<stan_model>(args_list);
  std::vector<double> params_r
    = cpp11::as_cpp<std::vector<double>>(args_list["upars"]);
  std::vector<int> params_i;
  std::vector<double> vars;
  boost::ecuyer1988 base_rng(static_cast<boost::uint32_t>(std::time(0)));

  ptr->write_array(base_rng, params_r, params_i, vars, false, false);
  cpp11::writable::doubles ret = std::move(vars);
  return ret;
}
