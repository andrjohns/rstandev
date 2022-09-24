#ifndef RSTANDEV_HELPERS_APPLY_SERVICE_FUNCTION_HPP
#define RSTANDEV_HELPERS_APPLY_SERVICE_FUNCTION_HPP

#include <services/hmc_nuts.hpp>
#include <helpers/extract_args.hpp>
#include <helpers/make_diag_writer.hpp>
#include <helpers/make_init_context.hpp>
#include <helpers/make_init_writer.hpp>
#include <helpers/make_sample_writer.hpp>
#include <unordered_map>

namespace rstandev {
  template <typename F>
  int apply_service_function(cpp11::writable::list args) {
    size_t num_chains = cpp11::as_cpp<size_t>(args["num_chains"]);
    size_t chain_id = cpp11::as_cpp<size_t>(args["chain_id"]);
    std::string output_dir = cpp11::as_cpp<std::string>(args["output_dir"]);

    cpp11::external_pointer<R_CheckUserInterrupt_Functor> interrupt_ptr(new rstandev::R_CheckUserInterrupt_Functor);
    cpp11::external_pointer<stan::callbacks::stream_logger> str_logger_ptr(new stan::callbacks::stream_logger(std::cout, std::cout, std::cout, std::cerr, std::cerr));

    args.push_back({cpp11::named_arg("init_contexts") = make_init_context<F>(args)});
    args.push_back({cpp11::named_arg("interrupt_ptr") = interrupt_ptr});
    args.push_back({cpp11::named_arg("str_logger_ptr") = str_logger_ptr});

    args.push_back({cpp11::named_arg("init_writer_ptr") = make_init_writer<F>(chain_id)});
    args.push_back({cpp11::named_arg("samp_writer_ptr") = make_sample_writer<F>(chain_id, output_dir)});
    args.push_back({cpp11::named_arg("diag_writer_ptr") = make_diag_writer<F>(chain_id)});

    return stan::math::apply(
      services_fun<F>(),
      extract_args(arg_types<F>(), args)
    );
  }
}

#endif
