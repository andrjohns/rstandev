sample_defaults <- function() {
  list(
    output = tempfile(),
    num_chains = 4,
    num_threads = 4,
    id = 1,
    random_seed = 1,
    init_radius = 2,
    num_warmup = 1,
    num_samples = 5,
    num_thin = 1,
    save_warmup = TRUE,
    refresh = 1,
    stepsize = 1.0,
    stepsize_jitter = 0.0,
    max_depth = 10,
    delta = 0.8,
    gamma = 0.05,
    kappa = 0.75,
    t0 = 10,
    init_buffer = 75,
    term_buffer = 50,
    window = 25
  )
}
