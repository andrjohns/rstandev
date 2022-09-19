
[[cpp11::register]]
void sample(cpp11::list args_list) {
  model_ptr_t model(args_list["model_ptr"]);
  std::string rdump_init = cpp11::as_cpp<std::string>(args_list["rdump_init"]);
  std::string output = cpp11::as_cpp<std::string>(args_list["output"]);
  size_t num_chains = cpp11::as_cpp<size_t>(args_list["num_chains"]);
  size_t num_threads = cpp11::as_cpp<size_t>(args_list["num_threads"]);

  std::shared_ptr<stan::io::var_context> init_ptr = var_context(rdump_init);
  std::vector<std::shared_ptr<stan::io::var_context>> init_contexts;
  for (int i = 0; i < num_chains; ++i) {
    init_contexts.push_back(init_ptr);
  }

  stan::math::init_threadpool_tbb(num_threads);

  R_CheckUserInterrupt_Functor interrupt;
  stan::callbacks::stream_logger logger(std::cout, std::cout, std::cout, std::cerr, std::cerr);

  std::vector<stan::callbacks::unique_stream_writer<std::ostream>>
      sample_writers;
  sample_writers.reserve(num_chains);
  std::vector<stan::callbacks::unique_stream_writer<std::ostream>>
      diagnostic_writers;
  diagnostic_writers.reserve(num_chains);
  std::vector<stan::callbacks::writer> init_writers{num_chains,
                                                    stan::callbacks::writer{}};

  for (int i = 0; i < num_chains; ++i) {
    auto output_filename = output + "_" + std::to_string(i) + ".csv";
    auto unique_fstream
        = std::make_unique<std::fstream>(output_filename, std::fstream::out);

    sample_writers.emplace_back(std::move(unique_fstream), "# ");
    diagnostic_writers.emplace_back(nullptr, "# ");
  }

  int return_code = stan::services::sample::hmc_nuts_dense_e_adapt(
    *model.get(),
    num_chains,
    init_contexts,
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
    interrupt,
    logger,
    init_writers,
    sample_writers,
    diagnostic_writers);
}
