
sample <- function(
  data,
  init = NULL,
  output_dir = tempdir(),
  num_chains = 4,
  num_threads = 1,
  random_seed = 0,
  num_samples = 2000,
  num_warmup = 2000,
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
  inv_metric = NULL,
  stepsize = 1,
  stepsize_jitter = 0,
  init_radius = 2,
  refresh = 200
  ) {

  input_args <- as.list(environment())

  service_name <- algorithm

  if (algorithm != "fixed_param") {
    service_name <- paste0(c(service_name, engine, metric), collapse = "_")
    if (adapt) {
      service_name <- paste0(c(service_name, "adapt"), collapse = "_")
    }
  }
  if (!is.null(init)) {
    input_args$rdump_init <- stan_rdump(init)
  }
  input_args$rdump_data <- stan_rdump(data)
  input_args$id <- 1
  if (!is.null(inv_metric)) {
    if (metric == "diag_e") {
      input_args$inv_metric <- paste0(
        "inv_metric <- c(",
        paste(diag(inv_metric), collapse = ","), ")"
      )
    } else {
      input_args$inv_metric <- stan_rdump(list(inv_metric=inv_metric))
    }
  }

  private$env$stan_methods_wrapper(service_name, input_args)
  model_ptr <- private$env$new_model(input_args$rdump_data, seed = random_seed)
  outputs <- paste0(input_args$output_dir, "/output", "_", 1:(num_chains), ".csv")
  stanfit$new(self, private, outputs, model_ptr)
}
