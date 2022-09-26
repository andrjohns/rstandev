
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

  #input_args$model_ptr <- private$env$new_model(stan_rdump(data), random_seed)
  #input_args$chain_id <- 1

  #num_cores <- min(num_chains, num_threads)
  #chain_threads <- num_threads %/% num_chains

  #dir <- file.path(tempdir(), "cpp11_src_files")
  #libpath <- file.path(dir, "src", paste0(private$dynlib_basename, private$dynlib_ext))
  #private$write_source(dir = dir)

  #cl <- future::makeClusterPSOCK(num_chains, verbose=TRUE)
  #future::plan(future::cluster, workers = cl)
  #furrr::future_walk(seq_len(num_chains), function(chain_id) {
  #  args <- input_args
  #  args$chain_id <- chain_id - 1
  #  args$num_chains <- 1
  #  args$num_threads <- chain_threads

  #  env <- new.env()
  #  source(file.path(dir, "R", "cpp11.R"), local = env)
  #  dyn.load(libpath, local = TRUE, now = TRUE)

  #  args$model_ptr <- env$new_model(stan_rdump(data), random_seed)
  #  env$stan_methods_wrapper(service_name, args)
  #})
  #parallel::stopCluster(cl)
  #future::plan(future::sequential)
  private$env$stan_methods_wrapper(service_name, input_args)
  outputs <- paste0(input_args$output_dir, "/output", "_", 0:(num_chains - 1), ".csv")
  stanfit$new(self, private, outputs, input_args$model_ptr)
}
