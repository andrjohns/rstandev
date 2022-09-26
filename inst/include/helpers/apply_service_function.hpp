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

  template <typename F, stan::require_all_same_t<F, NutsDiagAdaptT>* = nullptr>
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

    int id = 1;
    double init_radius = 2;
    int num_samples = 40000;
    int num_warmup = 40000;
    int num_thin = 1;
    bool save_warmup = false;
    int refresh = 100;
    double stepsize = 1;
    double stepsize_jitter = 0;
    int max_depth = 10;
    double delta = 0.8;
    double gamma = 0.05;
    double kappa = 0.75;
    double t0 = 10;
    size_t init_buffer = 75;
    size_t term_buffer = 50;
    size_t window = 25;
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
  auto name_iterator = [num_chains, id](auto i) {
    if (num_chains == 1) {
      return std::string("");
    } else {
      return std::string("_" + std::to_string(i + id));
    }
  };

    std::string output_name = output_dir + "/output";
    std::string output_ending = ".csv";
    std::string diagnostic_file = "";
  for (int i = 0; i < num_chains; i++) {
    auto output_filename = output_name + name_iterator(i) + output_ending;
    auto unique_fstream
        = std::make_unique<std::fstream>(output_filename, std::fstream::out);

    sample_writers.emplace_back(std::move(unique_fstream), "# ");
    /*if (diagnostic_file != "") {
      auto diagnostic_filename
          = diagnostic_name + name_iterator(i) + diagnostic_ending;
      diagnostic_writers.emplace_back(
          std::make_unique<std::fstream>(diagnostic_filename,
                                         std::fstream::out),
          "# ");
    } else {*/
      diagnostic_writers.emplace_back(nullptr, "# ");
    //}
  }
  /*
  for (int i = 0; i < num_chains; i++) {
    write_stan(sample_writers[i]);
    write_model(sample_writers[i], model.model_name());
    write_datetime(sample_writers[i]);
    parser.print(sample_writers[i]);
    write_parallel_info(sample_writers[i]);
    write_opencl_device(sample_writers[i]);
    write_compile_info(sample_writers[i], model_compile_info);
    write_stan(diagnostic_writers[i]);
    write_model(diagnostic_writers[i], model.model_name());
    parser.print(diagnostic_writers[i]);
  }*/
  /*
    tbb::parallel_for(
      tbb::blocked_range<size_t>(0, num_chains, 1),
      [num_warmup, num_samples, num_thin, save_warmup, num_chains,
       &model, &interrupt, &logger, &init_contexts, random_seed, init_radius,
       &sample_writers, refresh, stepsize, stepsize_jitter, max_depth, delta,
       gamma, kappa, t0, init_buffer, term_buffer, window, &init_writers,
       &diagnostic_writers](const tbb::blocked_range<size_t>& r) {
        for (size_t i = r.begin(); i != r.end(); ++i) {
          int res = stan::services::sample::hmc_nuts_diag_e_adapt(
            model, *(init_contexts[i]), random_seed, 1 + i, init_radius,
            num_warmup, num_samples, num_thin, save_warmup, refresh, stepsize,
            stepsize_jitter, max_depth, delta, gamma, kappa, t0, init_buffer,
            term_buffer, window, interrupt, logger, init_writers[i],
            sample_writers[i], diagnostic_writers[i]);
        }
      },
      tbb::simple_partitioner());
*/

  return stan::services::sample::hmc_nuts_diag_e_adapt(
            model, num_chains, init_contexts, random_seed, id, init_radius,
            num_warmup, num_samples, num_thin, save_warmup, refresh, stepsize,
            stepsize_jitter, max_depth, delta, gamma, kappa, t0, init_buffer,
            term_buffer, window, interrupt, logger, init_writers,
            sample_writers, diagnostic_writers);
  }

  template <typename F, stan::require_all_not_same_t<F, NutsDiagAdaptT>* = nullptr>
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
