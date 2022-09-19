
[[cpp11::register]]
void sample_test(SEXP model_ptr, std::string rdump_init, cpp11::sexp variable) {
  cpp11::external_pointer<stan_model> model(model_ptr);

  std::istringstream init_stream(rdump_init);
  stan::io::dump var_context(init_stream);
  std::shared_ptr<stan::io::var_context> init_ptr
    = std::make_shared<stan::io::dump>(var_context);
  std::vector<std::shared_ptr<stan::io::var_context>> init_contexts{init_ptr, init_ptr};

  int id = 1;
  unsigned int num_chains = 2;
  stan::math::init_threadpool_tbb(2);
  unsigned int random_seed = 1;
  double init_radius = 2;
  int num_warmup = 1;
  int num_samples = 5;
  int num_thin = 1;
  bool save_warmup = TRUE;
  int refresh = 1;
  double stepsize = 1.0;
  double stepsize_jitter = 0.0;
  int max_depth = 10;
  double delta = 0.8;
  double gamma = 0.05;
  double kappa = 0.75;
  double t0 = 10;
  unsigned int init_buffer = 75;
  unsigned int term_buffer = 50;
  unsigned int window = 25;
  R_CheckUserInterrupt_Functor interrupt;
  stan::callbacks::stream_logger logger(std::cout, std::cout, std::cout, std::cerr, std::cerr);
  stan::callbacks::writer init_writer;

  writer_t sample_writers;

  sample_writers.reserve(num_chains);
  std::vector<stan::callbacks::unique_stream_writer<std::ostream>>
    diagnostic_writers;
  diagnostic_writers.reserve(num_chains);
  std::vector<stan::callbacks::writer> init_writers{num_chains,
                                                    stan::callbacks::writer{}};
  for (int i = 0; i < num_chains; ++i) {
    auto unique_ostream
      = std::make_unique<std::ostringstream>("", std::stringstream::out);
    sample_writers.emplace_back(std::move(unique_ostream), "# ");
    diagnostic_writers.emplace_back(nullptr, "# ");
  }

  int return_code = stan::services::sample::hmc_nuts_dense_e_adapt(
    *model.get(), num_chains, init_contexts, random_seed, id, init_radius,
    num_warmup, num_samples, num_thin, save_warmup, refresh, stepsize,
    stepsize_jitter, max_depth, delta, gamma, kappa, t0, init_buffer,
    term_buffer, window, interrupt, logger, init_writers,
    sample_writers, diagnostic_writers);

    cpp11::writable::strings var(std::move(variable.data()));
    for (int i = 0; i < num_chains; ++i) {
      my_stringbuf t_buffer(sample_writers[i].get_stream().rdbuf());
      var[i] = std::move(t_buffer.my_str());
    }
}
