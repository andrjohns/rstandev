devtools::load_all("Stan Git/rstan.dev/")
st <- stanmodel$new("bernoulli.stan")
st$compile(allow_undefined = TRUE, external_cpp = "inc.hpp")
sf <- st$sample(
  data = list(N=2,y=0:1),
  init = list(theta = 0.1),
  metric = "dense_e",
  num_threads = 8,
  adapt = FALSE,
  num_chains = 4,
  output_dir = "tmp")


num_cores <- min(8,4)

