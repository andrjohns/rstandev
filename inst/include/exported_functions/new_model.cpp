
#include <helpers/new_model.hpp>

[[cpp11::register]]
SEXP new_model(std::string rdump_string,
                unsigned int seed = 0) {
  return rstandev::new_model<stan_model>(rdump_string, seed);
}
