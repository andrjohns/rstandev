
sample <- function(
  data, init,
  output_dir = tempdir(),
  num_chains = 4,
  num_threads = 1,
  random_seed = 0,
  num_samples = 1000,
  num_warmup = 1000,
  save_warmup = FALSE,
  num_thin = 1,
  adapt = TRUE,
  gamma = 0.05,
  delta = 0.8,
  kappa = 0.75,
  t0 = 10,
  init_buffer = 75,
  term_buffer = 50,
  window = 25,
  algorithm = "hmc",
  engine = "nuts",
  max_depth = 10,
  metric = "diag_e",
  metric_file = NULL,
  stepsize = 1,
  stepsize_jitter = 0,
  init_radius = 2,
  refresh = 10
  ) {

  input_args <- as.list(environment())

  service_name <- algorithm

  if (algorithm != "fixed_param") {
    service_name <- paste0(c(service_name, engine, metric), collapse = "_")
  }

  if (adapt) {
    service_name <- paste0(c(service_name, "adapt"), collapse = "_")
  }

  input_args$rdump_init <- stan_rdump(init)
  input_args$rdump_data <- stan_rdump(data)
  input_args$id <- 1

  private$env$stan_methods_wrapper(service_name, input_args)
  outputs <- paste0(input_args$output_dir, "/output", "_", 0:(num_chains - 1), ".csv")
  stanfit$new(self, private, outputs, input_args$model_ptr)
}
