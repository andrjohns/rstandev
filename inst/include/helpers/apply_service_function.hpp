#ifndef RSTANDEV_HELPERS_APPLY_SERVICE_FUNCTION_HPP
#define RSTANDEV_HELPERS_APPLY_SERVICE_FUNCTION_HPP

#include <helpers/service_functions.hpp>
#include <helpers/extract_args.hpp>
#include <helpers/make_inv_metric.hpp>
#include <unordered_map>

namespace rstandev {
namespace internal {
  template <typename ServiceFunT, typename ModelT, typename NumChainsT, typename InitContextsT,
            typename InvMetricsT, typename InterruptT,
            typename LoggerT, typename InitWritersT, typename SampleWritersT, typename DiagnosticWritersT,
            typename ArgsListT, stan::require_t<is_multi_chain<ServiceFunT>>* = nullptr>
  int apply_service_function_impl(ModelT&& model, NumChainsT&& num_chains, InitContextsT&& init_contexts,
                                  InvMetricsT&& inv_metrics, InterruptT&& interrupt, LoggerT&& logger, InitWritersT&& init_writers,
                                  SampleWritersT&& sample_writers, DiagnosticWritersT&& diagnostic_writers,
                                  ArgsListT&& args) {
    return stan::math::apply(
          services_fun<ServiceFunT>(),
          std::tuple_cat(
            std::forward_as_tuple(model, num_chains, init_contexts, inv_metrics),
            extract_args(arg_types<ServiceFunT>(), args),
            std::forward_as_tuple(interrupt, logger, init_writers, sample_writers, diagnostic_writers))
        );
  }

  template <typename ServiceFunT, typename ModelT, typename InitContextsT, typename InvMetricsT,
            stan::require_t<is_unit<ServiceFunT>>* = nullptr>
  decltype(auto) get_args_head(ModelT&& model, InitContextsT&& init_contexts, InvMetricsT&& inv_metrics) {
    return std::forward_as_tuple(model, init_contexts);
  }

  template <typename ServiceFunT, typename ModelT, typename InitContextsT, typename InvMetricsT,
            stan::require_not_t<is_unit<ServiceFunT>>* = nullptr>
  decltype(auto) get_args_head(ModelT&& model, InitContextsT&& init_contexts, InvMetricsT&& inv_metrics) {
    return std::forward_as_tuple(model, init_contexts, inv_metrics);
  }

  template <typename ServiceFunT, typename ModelT, typename NumChainsT, typename InitContextsT,
            typename InvMetricsT, typename InterruptT,
            typename LoggerT, typename InitWritersT, typename SampleWritersT, typename DiagnosticWritersT,
            typename ArgsListT, stan::require_not_t<is_multi_chain<ServiceFunT>>* = nullptr>
  int apply_service_function_impl(ModelT&& model, NumChainsT&& num_chains, InitContextsT&& init_contexts,
                                  InvMetricsT&& inv_metrics, InterruptT&& interrupt, LoggerT&& logger, InitWritersT&& init_writers,
                                  SampleWritersT&& sample_writers, DiagnosticWritersT&& diagnostic_writers,
                                  ArgsListT&& args) {
      decltype(auto) args_tuple = extract_args(arg_types<ServiceFunT>(), args);
      tbb::parallel_for(
        tbb::blocked_range<size_t>(0, num_chains, 1),
        [args_tuple, &model, &interrupt, &logger, &init_contexts, &sample_writers,
        &init_writers, &diagnostic_writers, &inv_metrics](const tbb::blocked_range<size_t>& r) {
          for (size_t i = r.begin(); i != r.end(); ++i) {
            int res = stan::math::apply(
              services_fun<ServiceFunT>(),
              std::tuple_cat(
                get_args_head<ServiceFunT>(
                  std::forward<decltype(model)>(model),
                  std::forward<decltype(*(init_contexts[i]))>(*(init_contexts[i])),
                  std::forward<decltype(*(inv_metrics[i]))>(*(inv_metrics[i]))),
                args_tuple,
                std::forward_as_tuple(interrupt, logger, init_writers[i], sample_writers[i], diagnostic_writers[i])));
          }
        },
        tbb::simple_partitioner());
      return 1;
  }
}

using context_vector = std::vector<std::shared_ptr<stan::io::var_context>>;
  template <typename F>
  int apply_service_function(cpp11::list args) {
    size_t num_chains = cpp11::as_cpp<size_t>(args["num_chains"]);
    std::string output_dir = cpp11::as_cpp<std::string>(args["output_dir"]);

    decltype(auto) var_cont = var_context(cpp11::as_cpp<std::string>(args["rdump_data"]));

    size_t random_seed = cpp11::as_cpp<size_t>(args["random_seed"]);

    decltype(auto) model = stan_model(
            *var_context(cpp11::as_cpp<std::string>(args["rdump_data"])),
            random_seed,
            &std::cout);
    std::stringstream stream("", std::stringstream::in);
    context_vector init_contexts = context_vector(num_chains, std::make_shared<stan::io::dump>(stan::io::dump(stream)));

    stan::callbacks::interrupt interrupt;
    stan::callbacks::stream_logger logger(std::cout, std::cout, std::cout,
                                          std::cerr, std::cerr);

    std::vector<stan::callbacks::unique_stream_writer<std::ostream>>
        sample_writers;
    sample_writers.reserve(num_chains);
    std::vector<stan::callbacks::unique_stream_writer<std::ostream>>
        diagnostic_writers;
    diagnostic_writers.reserve(num_chains);
    std::vector<stan::callbacks::writer> init_writers{num_chains,
                                                      stan::callbacks::writer{}};
    std::string output_name = output_dir + "/output";
    std::string output_ending = ".csv";
    std::string diagnostic_file = "";
    for (int i = 0; i < num_chains; i++) {
      auto output_filename = output_name + std::string("_" + std::to_string(i + 1)) + output_ending;
      auto unique_fstream
          = std::make_unique<std::fstream>(output_filename, std::fstream::out);

      sample_writers.emplace_back(std::move(unique_fstream), "# ");
      diagnostic_writers.emplace_back(nullptr, "# ");
    }

    std::vector<std::unique_ptr<stan::io::dump>> unit_e_metrics;
    unit_e_metrics.reserve(num_chains);
    if (!is_unit<F>::value) {
      for (size_t i = 0; i < num_chains; ++i) {
        unit_e_metrics.emplace_back(std::make_unique<stan::io::dump>(
            make_inv_metric<F>(model.num_params_r())));
      }
    }

    return internal::apply_service_function_impl<F>(
      std::forward<decltype(model)>(model),
      std::forward<size_t>(num_chains),
      std::forward<decltype(init_contexts)>(init_contexts),
      std::forward<decltype(unit_e_metrics)>(unit_e_metrics),
      std::forward<decltype(interrupt)>(interrupt),
      std::forward<decltype(logger)>(logger),
      std::forward<decltype(init_writers)>(init_writers),
      std::forward<decltype(sample_writers)>(sample_writers),
      std::forward<decltype(diagnostic_writers)>(diagnostic_writers),
      std::forward<decltype(args)>(args)
    );
  }
}

#endif
