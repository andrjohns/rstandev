devtools::load_all("Stan Git/rstan.dev/")
a <- Sys.time()
st <- stanmodel$new("bernoulli.stan")
st$compile()
sf <- st$sample(
  data = list(N=2,y=0:1),
  init = list(theta = 0.1),
  metric = "dense_e",
  engine = "static",
  algorithm = "fixed_param",
  inv_metric=diag(1),
  num_threads = 8,
  adapt = TRUE,
  num_chains = 4,
  num_warmup=40000,
  num_samples = 40000,
  refresh=40000)

sf <- st$optimize(
  data = list(N=2,y=0:1),
  algorithm = "Newton",
  num_chains = 4,
  num_threads = 4)
sf <- st$variational(
  data = list(N=2,y=0:1),
  algorithm = "meanfield",
  num_chains = 4,
  num_threads = 1)
Sys.time() -a

num_cores <- min(8,4)

