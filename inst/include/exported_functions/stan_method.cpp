
#include <helpers/apply_service_function.hpp>

[[cpp11::register]]
int stan_methods_wrapper(std::string name, cpp11::writable::list args) {
  static const std::map<std::string, std::function<int(cpp11::list)>> methods_index = {
    {"hmc_nuts_dense_e_adapt", rstandev::apply_service_function<rstandev::NutsDenseAdaptT>},
    {"hmc_nuts_dense_e", rstandev::apply_service_function<rstandev::NutsDenseT>},
    {"hmc_nuts_diag_e_adapt", rstandev::apply_service_function<rstandev::NutsDiagAdaptT>},
    {"hmc_nuts_diag_e", rstandev::apply_service_function<rstandev::NutsDiagAdaptT>}
  };
  return methods_index.at(name)(args);
}
