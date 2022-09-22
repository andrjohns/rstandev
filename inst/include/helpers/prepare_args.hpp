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
  template <typename F,
            stan::require_same_t<F, NutsDenseAdaptT>* = nullptr>
  auto prepare_args(const cpp11::list& args_list) {
    using ArgTypesTuple = std::tuple<
      cpp11::external_pointer<stan_model>, size_t,
      cpp11::external_pointer<std::vector<std::shared_ptr<stan::io::var_context>>>,
      size_t, int, double, int, int, int, bool, int, double, double, int, double,
      double, double, double, size_t, size_t, size_t,
      cpp11::external_pointer<R_CheckUserInterrupt_Functor>,
      cpp11::external_pointer<stan::callbacks::stream_logger>,
      cpp11::external_pointer<std::vector<stan::callbacks::writer>>,
      cpp11::external_pointer<std::vector<stan::callbacks::unique_stream_writer<std::ostream>>>,
      cpp11::external_pointer<std::vector<stan::callbacks::unique_stream_writer<std::ostream>>>
    >;
    static const char* ArgNames[26] = {
      "model_ptr", "num_chains", "init_contexts", "random_seed", "id", "init_radius",
      "num_warmup", "num_samples", "num_thin", "save_warmup", "refresh",
      "stepsize", "stepsize_jitter", "max_depth", "delta", "gamma", "kappa",
      "t0", "init_buffer", "term_buffer", "window", "interrupt_ptr",
      "str_logger_ptr", "init_writer_ptr", "samp_writer_ptr", "diag_writer_ptr"
    };
    return extract_args<ArgTypesTuple>(args_list, ArgNames);
  }

    template <typename F,
            stan::require_same_t<F, NutsDenseT>* = nullptr>
  auto prepare_args(const cpp11::list& args_list) {
    return std::forward_as_tuple(
      *cpp11::external_pointer<stan_model>(args_list["model_ptr"]).get(),
      *((*cpp11::external_pointer<std::vector<std::shared_ptr<stan::io::var_context>>>(args_list["init_contexts"]))[0]),
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
      *cpp11::external_pointer<R_CheckUserInterrupt_Functor>(args_list["interrupt_ptr"]),
      *cpp11::external_pointer<stan::callbacks::stream_logger>(args_list["str_logger_ptr"]),
      (*cpp11::external_pointer<std::vector<stan::callbacks::writer>>(args_list["init_writer_ptr"]))[0],
      (*cpp11::external_pointer<std::vector<stan::callbacks::unique_stream_writer<std::ostream>>>(args_list["samp_writer_ptr"]))[0],
      (*cpp11::external_pointer<std::vector<stan::callbacks::unique_stream_writer<std::ostream>>>(args_list["diag_writer_ptr"]))[0]);
  }
}

#endif
