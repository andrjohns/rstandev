#ifndef RSTANDEV_HELPERS_APPLY_SERVICE_FUNCTION_HPP
#define RSTANDEV_HELPERS_APPLY_SERVICE_FUNCTION_HPP

#include <services/hmc_nuts.hpp>
#include <helpers/extract_args.hpp>
#include <helpers/init_services.hpp>
#include <unordered_map>

namespace rstandev {
  template <typename F>
  int apply_service_function(cpp11::writable::list args) {
    auto services = init_services<F>(args);

    args.push_back({cpp11::named_arg("init_contexts") = std::get<0>(services)});
    args.push_back({cpp11::named_arg("interrupt_ptr") = std::get<1>(services)});
    args.push_back({cpp11::named_arg("str_logger_ptr") = std::get<2>(services)});
    args.push_back({cpp11::named_arg("init_writer_ptr") = std::get<3>(services)});
    args.push_back({cpp11::named_arg("samp_writer_ptr") = std::get<4>(services)});
    args.push_back({cpp11::named_arg("diag_writer_ptr") = std::get<5>(services)});

    return stan::math::apply(
      services_fun<F>(),
      extract_args(arg_types<F>(), args)
    );
  }
}

#endif
