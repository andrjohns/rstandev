#ifndef RSTANDEV_HELPERS_PREPARE_ARGS_HPP
#define RSTANDEV_HELPERS_PREPARE_ARGS_HPP

#include <helpers/method_typedefs.hpp>
#include <helpers/r_interrupt_functor.hpp>
#include <stan/io/var_context.hpp>
#include <stan/callbacks/stream_logger.hpp>
#include <stan/callbacks/stream_logger.hpp>
#include <stan/callbacks/stream_writer.hpp>
#include <stan/callbacks/unique_stream_writer.hpp>
#include <iostream>
#include <memory>

namespace rstandev {
  template <typename F,
            stan::require_same_t<F, NutsDenseAdaptT>* = nullptr>
  auto prepare_args(const cpp11::list& args_list) {
    return std::forward_as_tuple(
      *cpp11::external_pointer<stan_model>(args_list["model_ptr"]).get(),
      cpp11::as_cpp<size_t>(args_list["num_chains"]),
      *cpp11::external_pointer<std::vector<std::shared_ptr<stan::io::var_context>>>(args_list["init_contexts"]),
      cpp11::as_cpp<size_t>(args_list["random_seed"]),
      cpp11::as_cpp<int>(args_list["id"]),
      cpp11::as_cpp<double>(args_list["init_radius"]),
      cpp11::as_cpp<int>(args_list["num_warmup"]),
      cpp11::as_cpp<int>(args_list["num_samples"]),
      cpp11::as_cpp<int>(args_list["num_thin"]),
      cpp11::as_cpp<bool>(args_list["save_warmup"]),
      cpp11::as_cpp<int>(args_list["refresh"]),
      cpp11::as_cpp<double>(args_list["stepsize"]),
      cpp11::as_cpp<double>(args_list["stepsize_jitter"]),
      cpp11::as_cpp<int>(args_list["max_depth"]),
      cpp11::as_cpp<double>(args_list["delta"]),
      cpp11::as_cpp<double>(args_list["gamma"]),
      cpp11::as_cpp<double>(args_list["kappa"]),
      cpp11::as_cpp<double>(args_list["t0"]),
      cpp11::as_cpp<size_t>(args_list["init_buffer"]),
      cpp11::as_cpp<size_t>(args_list["term_buffer"]),
      cpp11::as_cpp<size_t>(args_list["window"]),
      *cpp11::external_pointer<R_CheckUserInterrupt_Functor>(args_list["interrupt_ptr"]),
      *cpp11::external_pointer<stan::callbacks::stream_logger>(args_list["str_logger_ptr"]),
      *cpp11::external_pointer<std::vector<stan::callbacks::writer>>(args_list["init_writer_ptr"]),
      *cpp11::external_pointer<std::vector<stan::callbacks::unique_stream_writer<std::ostream>>>(args_list["samp_writer_ptr"]),
      *cpp11::external_pointer<std::vector<stan::callbacks::unique_stream_writer<std::ostream>>>(args_list["diag_writer_ptr"]));
  }
}

#endif
