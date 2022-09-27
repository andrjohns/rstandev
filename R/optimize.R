
optimize <- function(
  data,
  init = NULL,
  algorithm = "BFGS",
  output_dir = tempdir(),
  num_chains = 4,
  num_threads = 1,
  random_seed = 0,
  num_iterations = 2000,
  save_iterations = TRUE,
  init_radius = 2,
  refresh = 200,
  history_size = 5,
  init_alpha = 0.001,
  tol_obj = 0.000000000001,
  tol_rel_obj = 10000.0,
  tol_grad = 10000000.0,
  tol_rel_grad = 10000000.0,
  tol_param = 0.00000001
  ) {

  input_args <- as.list(environment())

  service_name <- tolower(algorithm)

  if (!is.null(init)) {
    input_args$rdump_init <- stan_rdump(init)
  }
  input_args$rdump_data <- stan_rdump(data)
  input_args$id <- 1

  private$env$stan_methods_wrapper(service_name, input_args)
  model_ptr <- private$env$new_model(input_args$rdump_data, seed = random_seed)
  outputs <- paste0(input_args$output_dir, "/output", "_", 1:(num_chains), ".csv")
  stanfit$new(self, private, outputs, model_ptr)
}
