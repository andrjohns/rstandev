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

using shared_context_ptr = std::shared_ptr<stan::io::var_context>;
using context_vector = std::vector<shared_context_ptr>;
stan::model::model_base &new_model(stan::io::var_context &data_context,
                                   unsigned int seed, std::ostream *msg_stream);

  template <typename F, stan::require_t<is_multi_chain<F>>* = nullptr>
  int apply_service_function(cpp11::writable::list args) {
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

  return stan::math::apply(
      services_fun<F>(),
      std::tuple_cat(
        std::forward_as_tuple(model, num_chains, init_contexts),
        extract_args(arg_types<F>(), args),
        std::forward_as_tuple(interrupt, logger, init_writers, sample_writers, diagnostic_writers))
    );
  }

  template <typename F, stan::require_not_t<is_multi_chain<F>>* = nullptr>
  int apply_service_function(cpp11::writable::list args) {
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

    tbb::parallel_for(
      tbb::blocked_range<size_t>(0, num_chains, 1),
      [args, &model, &interrupt, &logger, &init_contexts, &sample_writers,
       &init_writers, &diagnostic_writers](const tbb::blocked_range<size_t>& r) {
        for (size_t i = r.begin(); i != r.end(); ++i) {
          int res = stan::math::apply(
            services_fun<F>(),
            std::tuple_cat(
              std::forward_as_tuple(model, *(init_contexts[i])),
              extract_args(arg_types<F>(), args),
              std::forward_as_tuple(interrupt, logger, init_writers[i], sample_writers[i], diagnostic_writers[i])));
        }
      },
      tbb::simple_partitioner());
      return 1;

  }
}

#endif
