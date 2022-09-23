#ifndef RSTANDEV_HELPERS_PREPARE_ARGS_HPP
#define RSTANDEV_HELPERS_PREPARE_ARGS_HPP

#include <helpers/method_typedefs.hpp>
#include <helpers/r_interrupt_functor.hpp>
#include <helpers/extract_args.hpp>
#include <stan/io/var_context.hpp>
#include <stan/callbacks/stream_logger.hpp>
#include <stan/callbacks/stream_logger.hpp>
#include <stan/callbacks/stream_writer.hpp>
#include <stan/callbacks/unique_stream_writer.hpp>
#include <iostream>
#include <memory>

namespace rstandev {
  template <typename T>
  class StanArg {
    public:
      T dummy;
      std::string arg_name;

      StanArg(std::string name) {
        arg_name = name;
      }
  };

  template <typename F, typename Enable = void>
  class StanArgsLookup {};

  template <typename F>
  class StanArgsLookup<F, stan::require_same_t<F, NutsDenseAdaptT>>  {
    public:
      inline static auto args() {
        return std::forward_as_tuple(
          StanArg<cpp11::external_pointer<stan_model>>("model_ptr"),
          StanArg<size_t>("num_chains"),
          StanArg<cpp11::external_pointer<std::vector<std::shared_ptr<stan::io::var_context>>>>("init_contexts"),
          StanArg<size_t>("random_seed"),
          StanArg<int>("id"),
          StanArg<double>("init_radius"),
          StanArg<int>("num_warmup"),
          StanArg<int>("num_samples"),
          StanArg<int>("num_thin"),
          StanArg<bool>("save_warmup"),
          StanArg<int>("refresh"),
          StanArg<double>("stepsize"),
          StanArg<double>("stepsize_jitter"),
          StanArg<int>("max_depth"),
          StanArg<double>("delta"),
          StanArg<double>("gamma"),
          StanArg<double>("kappa"),
          StanArg<double>("t0"),
          StanArg<size_t>("init_buffer"),
          StanArg<size_t>("term_buffer"),
          StanArg<size_t>("window"),
          StanArg<cpp11::external_pointer<R_CheckUserInterrupt_Functor>>("interrupt_ptr"),
          StanArg<cpp11::external_pointer<stan::callbacks::stream_logger>>("str_logger_ptr"),
          StanArg<cpp11::external_pointer<std::vector<stan::callbacks::writer>>>("init_writer_ptr"),
          StanArg<cpp11::external_pointer<std::vector<stan::callbacks::unique_stream_writer<std::ostream>>>>("samp_writer_ptr"),
          StanArg<cpp11::external_pointer<std::vector<stan::callbacks::unique_stream_writer<std::ostream>>>>("diag_writer_ptr")
        );
      }
  };

  template <typename F>
  class StanArgsLookup<F, stan::require_same_t<F, NutsDenseT>>  {
    public:
      inline static auto args() {
        return std::forward_as_tuple(
          StanArg<cpp11::external_pointer<stan_model>>("model_ptr"),
          StanArg<cpp11::external_pointer<std::shared_ptr<stan::io::var_context>>>("init_contexts"),
          StanArg<size_t>("random_seed"),
          StanArg<int>("id"),
          StanArg<double>("init_radius"),
          StanArg<int>("num_warmup"),
          StanArg<int>("num_samples"),
          StanArg<int>("num_thin"),
          StanArg<bool>("save_warmup"),
          StanArg<int>("refresh"),
          StanArg<double>("stepsize"),
          StanArg<double>("stepsize_jitter"),
          StanArg<int>("max_depth"),
          StanArg<cpp11::external_pointer<R_CheckUserInterrupt_Functor>>("interrupt_ptr"),
          StanArg<cpp11::external_pointer<stan::callbacks::stream_logger>>("str_logger_ptr"),
          StanArg<cpp11::external_pointer<stan::callbacks::writer>>("init_writer_ptr"),
          StanArg<cpp11::external_pointer<stan::callbacks::unique_stream_writer<std::ostream>>>("samp_writer_ptr"),
          StanArg<cpp11::external_pointer<stan::callbacks::unique_stream_writer<std::ostream>>>("diag_writer_ptr")
        );
      }
  };
}

#endif
