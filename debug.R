devtools::load_all("Stan Git/rstan.dev/")
a <- Sys.time()
st <- stanmodel$new("bernoulli.stan")
st$compile()
sf <- st$sample(
  data = list(N=2,y=0:1),
  init = list(theta = 0.1),
  metric = "unit_e",
  num_threads = 8,
  adapt = FALSE,
  num_chains = 4,
  num_warmup=40000,
  num_samples = 40000,
  refresh=40000)
Sys.time() -a

num_cores <- min(8,4)

