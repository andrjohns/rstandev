#ifndef RSTANDEV_HELPERS_INIT_SERVICES_HPP
#define RSTANDEV_HELPERS_INIT_SERVICES_HPP

#include <helpers/r_interrupt_functor.hpp>
#include <helpers/make_init_contexts.hpp>

namespace rstandev {
  template <typename F, stan::require_t<is_multi_chain<F>>* = nullptr>
  auto init_services(cpp11::writable::list args) {
    size_t num_chains = cpp11::as_cpp<size_t>(args["num_chains"]);

    cpp11::external_pointer<rstandev::R_CheckUserInterrupt_Functor> interrupt_ptr(new rstandev::R_CheckUserInterrupt_Functor);
    cpp11::external_pointer<stan::callbacks::stream_logger> str_logger_ptr(new stan::callbacks::stream_logger(std::cout, std::cout, std::cout, std::cerr, std::cerr));

    cpp11::external_pointer<std::vector<stan::callbacks::writer>> init_writer_ptr(
      new std::vector<stan::callbacks::writer>{num_chains, stan::callbacks::writer{}}
    );

    std::string output_dir = cpp11::as_cpp<std::string>(args["output_dir"]);

    cpp11::external_pointer<std::vector<stan::callbacks::unique_stream_writer<std::ostream>>>
    sample_writers(new std::vector<stan::callbacks::unique_stream_writer<std::ostream>>);
    sample_writers->reserve(num_chains);

    cpp11::external_pointer<std::vector<stan::callbacks::unique_stream_writer<std::ostream>>>
    diagnostic_writers(new std::vector<stan::callbacks::unique_stream_writer<std::ostream>>);
    diagnostic_writers->reserve(num_chains);


    for (int i = 0; i < num_chains; ++i) {
      auto output_filename = output_dir + "/output_" + std::to_string(i) + ".csv";
      auto unique_fstream
          = std::make_unique<std::fstream>(output_filename, std::fstream::out);

      sample_writers->emplace_back(std::move(unique_fstream), "# ");
      diagnostic_writers->emplace_back(nullptr, "# ");
    }

    return std::make_tuple(
      rstandev::make_init_contexts(args),
      interrupt_ptr,
      str_logger_ptr,
      init_writer_ptr,
      sample_writers,
      diagnostic_writers
    );
  }

  template <typename F, stan::require_not_t<is_multi_chain<F>>* = nullptr>
  auto init_services(cpp11::writable::list args) {
    size_t num_chains = cpp11::as_cpp<size_t>(args["num_chains"]);
    size_t chain_id = cpp11::as_cpp<size_t>(args["chain_id"]);

    cpp11::external_pointer<rstandev::R_CheckUserInterrupt_Functor> interrupt_ptr(new rstandev::R_CheckUserInterrupt_Functor);
    cpp11::external_pointer<stan::callbacks::stream_logger> str_logger_ptr(new stan::callbacks::stream_logger(std::cout, std::cout, std::cout, std::cerr, std::cerr));

    cpp11::external_pointer<stan::callbacks::writer> init_writer_ptr(new stan::callbacks::writer);

    std::string output_dir = cpp11::as_cpp<std::string>(args["output_dir"]);
      auto output_filename = output_dir + "/output_" + std::to_string(chain_id - 1) + ".csv";
      auto unique_fstream
          = std::make_unique<std::fstream>(output_filename, std::fstream::out);

    cpp11::external_pointer<stan::callbacks::unique_stream_writer<std::ostream>>
    sample_writers(new stan::callbacks::unique_stream_writer<std::ostream>(std::move(unique_fstream), "# "));

    cpp11::external_pointer<stan::callbacks::unique_stream_writer<std::ostream>>
    diagnostic_writers(new stan::callbacks::unique_stream_writer<std::ostream>(nullptr, "# "));

    std::string rdump_init = cpp11::as_cpp<std::string>(args["rdump_init"]);

    cpp11::external_pointer<std::shared_ptr<stan::io::var_context>> init_contexts(
      new std::shared_ptr<stan::io::var_context>(var_context(rdump_init))
    );

    return std::make_tuple(
      init_contexts,
      interrupt_ptr,
      str_logger_ptr,
      init_writer_ptr,
      sample_writers,
      diagnostic_writers
    );
  }
}

#endif